#include "vtcn/crank/GapDetector.hpp"
#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

#include <gtest/gtest.h>

#include <cstddef>
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
    const auto revolution_period_us = static_cast<Interval::rep>(60'000'000ULL / rpm.value);
    const auto revolution_period = Interval{revolution_period_us};
    return revolution_period / kToothPositionsPerRevolution;
}

[[nodiscard]] auto make_config(std::uint32_t rpm, std::size_t revolution_count,
                               Timestamp start_time = Timestamp{0}) -> CrankSignalConfig {
    return CrankSignalConfig{
        .target_rpm = Rpm{rpm},
        .revolution_count = Revolutions{revolution_count},
        .start_time = start_time,
    };
}

[[nodiscard]] auto jitter_offset_for(const std::size_t pulse_index) -> Timestamp {
    switch (pulse_index % 6U) {
    case 0U:
        return Timestamp{-20};
    case 1U:
        return Timestamp{15};
    case 2U:
        return Timestamp{-10};
    case 3U:
        return Timestamp{25};
    case 4U:
        return Timestamp{-15};
    default:
        return Timestamp{10};
    }
}

[[nodiscard]] auto apply_light_jitter(const std::vector<PulseEvent> &pulses) -> std::vector<PulseEvent> {
    std::vector<PulseEvent> jittered{pulses};

    for (std::size_t i = 0; i < jittered.size(); ++i) {
        jittered[i].timestamp += jitter_offset_for(i);
    }

    return jittered;
}

TEST(GapDetectorTest, DetectsGapInCleanMultiRevolutionInput) {
    const auto config = make_config(600, 2);

    const SimulatedCrankPulseSource source{};
    const auto pulses = source.generate(config);

    const auto expected_nominal_interval = nominal_tooth_interval(config.target_rpm);
    const auto expected_gap_interval = expected_nominal_interval * 2;

    const GapDetector detector{};
    const auto result = detector.detect(pulses);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->gap_index, kEmittedPulsesPerRevolution);
    EXPECT_EQ(result->gap_interval, expected_gap_interval);
}

TEST(GapDetectorTest, DetectsGapUnderLightJitter) {
    const auto config = make_config(600, 2);

    const SimulatedCrankPulseSource source{};
    const auto clean_pulses = source.generate(config);
    const auto pulses = apply_light_jitter(clean_pulses);

    for (std::size_t i = 1; i < pulses.size(); ++i) {
        ASSERT_GT(pulses[i].timestamp, pulses[i - 1].timestamp);
    }

    const auto expected_gap_interval =
        pulses[kEmittedPulsesPerRevolution].timestamp -
        pulses[kEmittedPulsesPerRevolution - 1].timestamp;

    const GapDetector detector{};
    const auto result = detector.detect(pulses);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->gap_index, kEmittedPulsesPerRevolution);
    EXPECT_EQ(result->gap_interval, expected_gap_interval);
}

TEST(GapDetectorTest, ReturnsNulloptForTooFewPulses) {
    const std::vector<PulseEvent> pulses{
        PulseEvent{.timestamp = Timestamp{1'000}, .revolution_index = 0U, .tooth_index = 0U},
        PulseEvent{.timestamp = Timestamp{2'000}, .revolution_index = 0U, .tooth_index = 1U},
    };

    const GapDetector detector{};
    const auto result = detector.detect(pulses);

    EXPECT_FALSE(result.has_value());
}

TEST(GapDetectorTest, ReturnsNulloptForNonMonotonicTimestamps) {
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
    // The largest interval must clear the detector's strict threshold.
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
