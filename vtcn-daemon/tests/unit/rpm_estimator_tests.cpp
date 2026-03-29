#include "vtcn/crank/GapDetector.hpp"
#include "vtcn/crank/RpmEstimator.hpp"
#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace {

using vtcn::crank::CrankSignalConfig;
using vtcn::crank::GapDetectionResult;
using vtcn::crank::GapDetector;
using vtcn::crank::Interval;
using vtcn::crank::PulseEvent;
using vtcn::crank::Revolutions;
using vtcn::crank::Rpm;
using vtcn::crank::RpmEstimator;
using vtcn::crank::SimulatedCrankPulseSource;
using vtcn::crank::Timestamp;

constexpr std::size_t kToothPositionsPerRevolution = 36;

[[nodiscard]] auto make_config(std::uint32_t rpm, std::size_t revolution_count,
                               Timestamp start_time = Timestamp{0}) -> CrankSignalConfig {
    return CrankSignalConfig{
        .target_rpm = Rpm{rpm},
        .revolution_count = Revolutions{revolution_count},
        .start_time = start_time,
    };
}

[[maybe_unused]] auto error_percentage(const Rpm estimated_rpm, const Rpm target_rpm)
    -> std::uint32_t {
    const auto difference = (estimated_rpm.value >= target_rpm.value)
        ? (estimated_rpm.value - target_rpm.value)
        : (target_rpm.value - estimated_rpm.value);
    return static_cast<std::uint32_t>((difference * 100U) / target_rpm.value);
}

[[nodiscard]] auto nominal_tooth_interval(const Rpm rpm) -> Interval {
    const auto revolution_period_us = static_cast<Interval::rep>(60'000'000ULL / rpm.value);
    const auto revolution_period = Interval{revolution_period_us};
    return revolution_period / kToothPositionsPerRevolution;
}

[[nodiscard]] auto average_adjacent_intervals(std::span<const PulseEvent> pulses) -> Interval {
    Interval total_interval{0};

    for (std::size_t i = 1; i < pulses.size(); ++i) {
        total_interval += pulses[i].timestamp - pulses[i - 1].timestamp;
    }

    return total_interval / (pulses.size() - 1U);
}

[[nodiscard]] auto average_non_gap_adjacent_intervals(std::span<const PulseEvent> pulses,
                                                      const std::size_t gap_index) -> Interval {
    Interval total_nominal_interval{0};
    std::size_t nominal_interval_count = 0U;

    for (std::size_t i = 1; i < pulses.size(); ++i) {
        if (i == gap_index) {
            continue;
        }

        total_nominal_interval += pulses[i].timestamp - pulses[i - 1].timestamp;
        ++nominal_interval_count;
    }

    return total_nominal_interval / nominal_interval_count;
}

TEST(RpmEstimatorTest, ReturnsEstimateForValidCleanInput) {
    const auto config = make_config(600, 2);

    const SimulatedCrankPulseSource source{};
    const auto pulses = source.generate(config);
    const GapDetector detector{};
    const auto gap_result = detector.detect(pulses);

    ASSERT_TRUE(gap_result.has_value());

    const RpmEstimator estimator{};
    const auto estimate = estimator.estimate(pulses, *gap_result);

    ASSERT_TRUE(estimate.has_value());
}

TEST(RpmEstimatorTest, IgnoresMissingToothGapWhenComputingNominalInterval) {
    const auto config = make_config(600, 2);

    const SimulatedCrankPulseSource source{};
    const auto pulses = source.generate(config);
    const GapDetector detector{};
    const auto gap_result = detector.detect(pulses);

    ASSERT_TRUE(gap_result.has_value());

    const auto expected_nominal_interval = nominal_tooth_interval(config.target_rpm);
    const auto expected_average_non_gap_interval =
        average_non_gap_adjacent_intervals(pulses, gap_result->gap_index);

    // Including the long gap would bias the nominal interval upward.
    const auto biased_average_interval = average_adjacent_intervals(pulses);
    const auto observed_gap_interval =
        pulses[gap_result->gap_index].timestamp - pulses[gap_result->gap_index - 1U].timestamp;

    const RpmEstimator estimator{};
    const auto estimate = estimator.estimate(pulses, *gap_result);

    ASSERT_TRUE(estimate.has_value());
    EXPECT_EQ(expected_average_non_gap_interval, expected_nominal_interval);
    EXPECT_EQ(estimate->nominal_tooth_interval, expected_average_non_gap_interval);
    EXPECT_GT(observed_gap_interval, estimate->nominal_tooth_interval);
    EXPECT_NE(estimate->nominal_tooth_interval, biased_average_interval);
    EXPECT_EQ(estimate->estimated_rpm.value, config.target_rpm.value);
}

TEST(RpmEstimatorTest, KeepsRpmErrorWithinToleranceForRepresentativeInputs) {
    struct PreparedCase final {
        Rpm target_rpm{};
        std::vector<PulseEvent> pulses{};
        GapDetectionResult gap_result{};
        Interval expected_nominal_tooth_interval{};
    };

    const SimulatedCrankPulseSource source{};
    const RpmEstimator estimator{};
    const std::vector<std::uint32_t> target_rpm_values{350, 1500, 5000};

    std::vector<PreparedCase> cases;
    cases.reserve(target_rpm_values.size());

    for (const auto target_rpm : target_rpm_values) {
        const auto config = make_config(target_rpm, 2);
        const auto pulses = source.generate(config);
        const GapDetector detector{};
        const auto gap_result = detector.detect(pulses);

        ASSERT_TRUE(gap_result.has_value());

        cases.push_back(PreparedCase{
            .target_rpm = Rpm{target_rpm},
            .pulses = pulses,
            .gap_result = *gap_result,
            .expected_nominal_tooth_interval =
                average_non_gap_adjacent_intervals(pulses, gap_result->gap_index),
        });
    }

    for (const auto &test_case : cases) {
        constexpr std::uint32_t kAllowedErrorPercent = 1U;

        const auto estimate = estimator.estimate(test_case.pulses, test_case.gap_result);

        ASSERT_TRUE(estimate.has_value());

        const auto percent_error =
            error_percentage(estimate->estimated_rpm, test_case.target_rpm);

        EXPECT_EQ(estimate->nominal_tooth_interval, test_case.expected_nominal_tooth_interval);
        EXPECT_LE(percent_error, kAllowedErrorPercent);
    }
}

TEST(RpmEstimatorTest, ReturnsNulloptForTooFewPulses) {
    const std::vector<PulseEvent> pulses{
        PulseEvent{.timestamp = Timestamp{1'000}, .revolution_index = 0U, .tooth_index = 0U},
        PulseEvent{.timestamp = Timestamp{2'000}, .revolution_index = 0U, .tooth_index = 1U},
    };

    const GapDetectionResult gap_result{
        .gap_index = 1U,
        .gap_interval = Interval{1'000},
    };

    const RpmEstimator estimator{};
    const auto result = estimator.estimate(pulses, gap_result);

    EXPECT_FALSE(result.has_value());
}

TEST(RpmEstimatorTest, ReturnsNulloptForNonMonotonicTimestamps) {
    const std::vector<PulseEvent> pulses{
        PulseEvent{.timestamp = Timestamp{1'000}, .revolution_index = 0U, .tooth_index = 0U},
        PulseEvent{.timestamp = Timestamp{2'000}, .revolution_index = 0U, .tooth_index = 1U},
        PulseEvent{.timestamp = Timestamp{2'000}, .revolution_index = 0U, .tooth_index = 2U},
    };

    const GapDetectionResult gap_result{
        .gap_index = 2U,
        .gap_interval = Interval{1'000},
    };

    const RpmEstimator estimator{};
    const auto result = estimator.estimate(pulses, gap_result);

    EXPECT_FALSE(result.has_value());
}

TEST(RpmEstimatorTest, ReturnsNulloptForGapIndexZero) {
    // `gap_index` names the later pulse of an adjacent interval.
    const auto config = make_config(600, 2);

    const SimulatedCrankPulseSource source{};
    const auto pulses = source.generate(config);
    const GapDetectionResult gap_result{
        .gap_index = 0U,
        .gap_interval = Interval{1'000},
    };

    const RpmEstimator estimator{};
    const auto result = estimator.estimate(pulses, gap_result);

    EXPECT_FALSE(result.has_value());
}

TEST(RpmEstimatorTest, ReturnsNulloptForGapIndexAtOrPastPulseCount) {
    const auto config = make_config(600, 2);

    const SimulatedCrankPulseSource source{};
    const auto pulses = source.generate(config);
    const GapDetectionResult gap_result{
        .gap_index = pulses.size(),
        .gap_interval = Interval{1'000},
    };

    const RpmEstimator estimator{};
    const auto result = estimator.estimate(pulses, gap_result);

    EXPECT_FALSE(result.has_value());
}

} // namespace
