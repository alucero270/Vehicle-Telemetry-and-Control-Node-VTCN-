#include "vtcn/crank/GapDetector.hpp"
#include "vtcn/crank/RpmEstimator.hpp"
#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

#include <gtest/gtest.h>

#include <cstdint>
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
constexpr std::size_t kMissingToothCount = 1;
constexpr std::size_t kEmittedPulsesPerRevolution =
    kToothPositionsPerRevolution - kMissingToothCount;

[[nodiscard]] auto make_config(std::uint32_t rpm, std::size_t revolution_count,
                               Timestamp start_time = Timestamp{0}) -> CrankSignalConfig {
    // Keep test setup terse and consistent so each test stays focused on one behavior.
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

TEST(RpmEstimatorTest, IgnoresMissingToothGapWhenComputingNominalInterval) {
    // Arrange:
    // - clean multi-revolution 36-1 input
    // - detect the missing-tooth gap
    // - expected nominal interval is the regular tooth interval

    // Act:
    // - estimate RPM

    // Assert:
    // - result has value
    // - nominal_tooth_interval equals expected nominal interval
    // - estimate is not biased by the long gap
}

TEST(RpmEstimatorTest, KeepsRpmErrorWithinToleranceForRepresentativeInputs) {
    struct PreparedCase final {
        Rpm target_rpm{};
        std::vector<PulseEvent> pulses{};
        GapDetectionResult gap_result{};
    };

    constexpr std::uint32_t kAllowedErrorPercent = 1U;

    const SimulatedCrankPulseSource source{};
    const GapDetector detector{};
    const RpmEstimator estimator{};
    const std::vector<std::uint32_t> target_rpms{350, 1500, 5000};

    std::vector<PreparedCase> cases;
    cases.reserve(target_rpms.size());

    for (const auto target_rpm : target_rpms) {
        const auto config = make_config(target_rpm, 2);
        const auto pulses = source.generate(config);
        const auto gap_result = detector.detect(pulses);

        ASSERT_TRUE(gap_result.has_value());

        cases.push_back(PreparedCase{
            .target_rpm = Rpm{target_rpm},
            .pulses = pulses,
            .gap_result = *gap_result,
        });
    }

    for (const auto &test_case : cases) {
        const auto estimate = estimator.estimate(test_case.pulses, test_case.gap_result);

        ASSERT_TRUE(estimate.has_value());

        const auto percent_error =
            error_percentage(estimate->estimated_rpm, test_case.target_rpm);

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

TEST(RpmEstimatorTest, ReturnsNulloptForInvalidGapIndex) {
    // Arrange:
    // - valid pulses with impossible gap metadata

    // Act / Assert:
    // - estimate returns nullopt
}

} // namespace
