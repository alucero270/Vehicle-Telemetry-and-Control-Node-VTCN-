#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

#include <gtest/gtest.h>

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
    // Keep test setup terse and consistent so each test stays focused on one behavior.
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
    // Gap shape is validated from adjacent pulse timestamps rather than from synthetic gap events.
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

[[nodiscard]] auto nominal_tooth_interval(const Rpm rpm) -> Interval {
    // The generator uses integer microsecond timing, so the expected tooth interval
    // is derived from one minute in microseconds divided by RPM, then by 36 positions.
    const auto revolution_period_us =
        static_cast<Interval::rep>(60'000'000ULL / rpm.value);
    const auto revolution_period = Interval{revolution_period_us};
    return revolution_period / kToothPositionsPerRevolution;
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

TEST(SimulatedCrankPulseSourceTest, EmitsOneObservableLongGapPerObservableRevolutionBoundary) {
    // The generator models one missing-tooth gap per revolution, but adjacent
    // intervals only expose the gaps that sit between emitted pulses already in
    // the finite output sequence. That means an N-revolution pulse vector shows
    // exactly N - 1 observable long inter-revolution gaps.
    const auto revolution_count = std::size_t{4};
    const auto config = make_config(600, revolution_count);
    const SimulatedCrankPulseSource source{};

    const auto pulses = source.generate(config);
    const auto intervals = intervals_between(pulses);
    const auto nominal_interval = nominal_tooth_interval(config.target_rpm);
    const auto missing_tooth_gap = nominal_interval * 2;

    ASSERT_EQ(intervals.size(), pulses.size() - 1);

    std::size_t long_gap_count = 0;
    for (const auto interval : intervals) {
        if (interval == missing_tooth_gap) {
            ++long_gap_count;
        }
    }

    // Each observable boundary between two generated revolutions contributes one
    // oversized interval. The final revolution's trailing missing-tooth gap is
    // not represented because no subsequent emitted pulse exists in this finite
    // sequence to close that last interval.
    EXPECT_EQ(long_gap_count, revolution_count - 1);
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
