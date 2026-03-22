#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <span>
#include <vector>

namespace {

using vtcn::crank::CrankSignalConfig;
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

[[nodiscard]] auto make_config(std::uint32_t rpm, std::size_t revolution_count,
                               Timestamp start_time = Timestamp{0}) -> CrankSignalConfig {
    return CrankSignalConfig{
        .target_rpm = Rpm{rpm},
        .revolution_count = Revolutions{revolution_count},
        .start_time = start_time,
    };
}

[[nodiscard]] auto generate_pulses(std::uint32_t rpm, std::size_t revolution_count,
                                   Timestamp start_time = Timestamp{0}) -> std::vector<PulseEvent> {
    const SimulatedCrankPulseSource source{};
    return source.generate(make_config(rpm, revolution_count, start_time));
}

[[nodiscard]] auto intervals_between(std::span<const PulseEvent> pulses) -> std::vector<Interval> {
    std::vector<Interval> intervals;
    if (pulses.size() < 2) {
        return intervals;
    }

    intervals.reserve(pulses.size() - 1);
    for (std::size_t i = 1; i < pulses.size(); ++i) {
        intervals.push_back(pulses[i].timestamp - pulses[i - 1].timestamp);
    }

    return intervals;
}

TEST(SimulatedCrankPulseSourceTest, GeneratesThirtyFivePulsesPerRevolution) {
    const auto revolution_count = std::size_t{3};
    const auto pulses = generate_pulses(1200, revolution_count);

    EXPECT_EQ(pulses.size(), kEmittedPulsesPerRevolution * revolution_count);
}

TEST(SimulatedCrankPulseSourceTest, TimestampsAreStrictlyIncreasing) {
    const auto pulses = generate_pulses(1200, 2);

    ASSERT_FALSE(pulses.empty());

    for (std::size_t i = 1; i < pulses.size(); ++i) {
        EXPECT_GT(pulses[i].timestamp, pulses[i - 1].timestamp);
    }
}

TEST(SimulatedCrankPulseSourceTest, ProducesDeterministicOutputForIdenticalInputs) {
    const auto first = generate_pulses(900, 2, Timestamp{5'000});
    const auto second = generate_pulses(900, 2, Timestamp{5'000});

    ASSERT_EQ(first.size(), second.size());

    for (std::size_t i = 0; i < first.size(); ++i) {
        EXPECT_EQ(first[i].timestamp, second[i].timestamp);
        EXPECT_EQ(first[i].revolution_index, second[i].revolution_index);
        EXPECT_EQ(first[i].tooth_index, second[i].tooth_index);
    }
}

TEST(SimulatedCrankPulseSourceTest, TracksRevolutionAndToothIndices) {
    const auto pulses = generate_pulses(1000, 1);

    ASSERT_EQ(pulses.size(), kEmittedPulsesPerRevolution);

    for (std::size_t i = 0; i < pulses.size(); ++i) {
        EXPECT_EQ(pulses[i].revolution_index, 0U);
        EXPECT_EQ(pulses[i].tooth_index, i);
    }
}

} // namespace