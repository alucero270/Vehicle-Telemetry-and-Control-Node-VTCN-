#include "vtcn/crank/GapDetector.hpp"
#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

namespace {
using vtcn::crank::CrankSignalConfig;
using vtcn::crank::GapDetector;
using vtcn::crank::Interval;
using vtcn::crank::PulseEvent;
using vtcn::crank::Revolutions;
using vtcn::crank::Rpm;
using vtcn::crank::SimulatedCrankPulseSource;
using vtcn::crank::Timestamp;

constexpr std::size_t kToothPositionsPerRevolution = 36;
constexpr std::size_t kMissingToothCount = 1;
constexpr std::size_t kEmittedPulsesPerRevolution =
    kToothPositionsPerRevolution - kMissingToothCount;

[[nodiscard]] auto nominal_tooth_interval(const Rpm rpm) -> Interval {
    // The generator uses integer microsecond timing, so the expected tooth interval
    // is derived from one minute in microseconds divided by RPM, then by 36 positions.
    const auto revolution_period_us = static_cast<Interval::rep>(60'000'000ULL / rpm.value);
    const auto revolution_period = Interval{revolution_period_us};
    return revolution_period / kToothPositionsPerRevolution;
}

[[nodiscard]] auto make_config(std::uint32_t rpm, std::size_t revolution_count,
                               Timestamp start_time = Timestamp{0}) -> CrankSignalConfig {
    // Keep test setup terse and consistent so each test stays focused on one behavior.
    return CrankSignalConfig{
        .target_rpm = Rpm{rpm},
        .revolution_count = Revolutions{revolution_count},
        .start_time = start_time,
    };
}

TEST(GapDetectorTest, DetectsGapInCleanMultiRevolutionInput) {
    // Two revolutions produce one observable inter-revolution missing-tooth gap,
    // which keeps the expected gap location unambiguous.
    const auto config = make_config(600, 2);

    const SimulatedCrankPulseSource source{};
    const auto pulses = source.generate(config);

    const auto expected_nominal_interval = nominal_tooth_interval(config.target_rpm);
    // The deterministic 36-1 generator models the missing-tooth interval as
    // exactly two nominal tooth intervals.
    const auto expected_gap_interval = expected_nominal_interval * 2;

    const GapDetector detector{};
    const auto result = detector.detect(pulses);

    ASSERT_TRUE(result.has_value());
    // The long interval sits between the last pulse of revolution 0 and the
    // first pulse of revolution 1, so the later pulse index is 35.
    EXPECT_EQ(result->gap_index, kEmittedPulsesPerRevolution);
    EXPECT_EQ(result->gap_interval, expected_gap_interval);
}

TEST(GapDetectorTest, ReturnsNulloptForTooFewPulses) {
    // Two pulses expose only one interval, which is below the Phase 0 minimum.
    const std::vector<PulseEvent> pulses{
        PulseEvent{.timestamp = Timestamp{1'000}, .revolution_index = 0U, .tooth_index = 0U},
        PulseEvent{.timestamp = Timestamp{2'000}, .revolution_index = 0U, .tooth_index = 1U},
    };

    const GapDetector detector{};
    const auto result = detector.detect(pulses);

    EXPECT_FALSE(result.has_value());
}

TEST(GapDetectorTest, ReturnsNulloptForNonMonotonicTimestamps) {
    // Equal timestamps are invalid because the detector requires strict
    // monotonic increase before it computes adjacent intervals.
    const std::vector<PulseEvent> pulses{
        PulseEvent{.timestamp = Timestamp{1'000}, .revolution_index = 0U, .tooth_index = 0U},
        PulseEvent{.timestamp = Timestamp{2'000}, .revolution_index = 0U, .tooth_index = 1U},
        PulseEvent{.timestamp = Timestamp{2'000}, .revolution_index = 0U, .tooth_index = 2U},
    };

    const GapDetector detector{};
    const auto result = detector.detect(pulses);

    EXPECT_FALSE(result.has_value());
}

TEST(GapDetectorTest, ReturnsNulloptWhenLargestIntervalDoesNotExceedThreshold) {
    // Valid monotonic input must still return no detection when the largest
    // interval does not clear the strict integer threshold.
    const std::vector<PulseEvent> pulses{
        PulseEvent{.timestamp = Timestamp{1'000}, .revolution_index = 0U, .tooth_index = 0U},
        PulseEvent{.timestamp = Timestamp{2'000}, .revolution_index = 0U, .tooth_index = 1U},
        PulseEvent{.timestamp = Timestamp{3'200}, .revolution_index = 0U, .tooth_index = 2U},
    };

    const GapDetector detector{};
    const auto result = detector.detect(pulses);

    EXPECT_FALSE(result.has_value());
}

TEST(GapDetectorTest, IdenticalInputProducesIdenticalOutput) {
    // Re-running the pure detector on the same pulse sequence must yield the
    // same location and interval every time.
    const auto config = make_config(600, 2, Timestamp{5'000});

    const SimulatedCrankPulseSource source{};
    const auto pulses = source.generate(config);
    const GapDetector detector{};

    const auto first = detector.detect(pulses);
    const auto second = detector.detect(pulses);

    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(first->gap_index, second->gap_index);
    EXPECT_EQ(first->gap_interval, second->gap_interval);
}
} // namespace
