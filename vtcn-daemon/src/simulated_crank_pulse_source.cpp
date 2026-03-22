#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

#include <stdexcept>

namespace {

[[nodiscard]] auto emitted_pulses_per_revolution(const vtcn::crank::CrankWheelProfile &profile)
    -> std::size_t {
    // A 36-1 wheel has 36 tooth positions but only 35 emitted pulse events per revolution.
    return profile.tooth_positions_per_revolution - profile.missing_tooth_count;
}

[[nodiscard]] auto nominal_tooth_interval(const vtcn::crank::Rpm rpm,
                                          const vtcn::crank::CrankWheelProfile &profile)
    -> vtcn::crank::Interval {
    if (rpm.value == 0U) {
        throw std::invalid_argument{"Cannot calculate nominal tooth interval for zero RPM"};
    }

    // Host-side simulation uses integer microseconds so identical inputs produce identical
    // timestamps without floating-point drift.
    const auto revolution_period_us =
        static_cast<vtcn::crank::Interval::rep>(60'000'000ULL / rpm.value);
    const auto revolution_period = vtcn::crank::Interval{revolution_period_us};

    return revolution_period / profile.tooth_positions_per_revolution;
}

} // namespace

namespace vtcn::crank {

SimulatedCrankPulseSource::SimulatedCrankPulseSource(CrankWheelProfile profile) noexcept
    : profile_(profile) {}

const CrankWheelProfile &SimulatedCrankPulseSource::profile() const noexcept {
    return profile_;
}

std::vector<PulseEvent> SimulatedCrankPulseSource::generate(const CrankSignalConfig &config) const {
    if (profile_.tooth_positions_per_revolution == 0U ||
        profile_.missing_tooth_count >= profile_.tooth_positions_per_revolution) {
        throw std::invalid_argument{"Invalid crank wheel profile"};
    }

    const auto pulses_per_revolution = emitted_pulses_per_revolution(profile_);
    const auto total_pulses = pulses_per_revolution * config.revolution_count.value;
    const auto tooth_interval = nominal_tooth_interval(config.target_rpm, profile_);
    const auto missing_tooth_multiplier =
        static_cast<Interval::rep>(profile_.missing_tooth_count + 1U);
    // Represent the missing tooth as one longer interval in time, not as a synthetic pulse.
    const auto missing_tooth_gap = tooth_interval * missing_tooth_multiplier;

    std::vector<PulseEvent> pulses;
    pulses.reserve(total_pulses);

    auto current_timestamp = config.start_time;

    for (std::size_t revolution = 0; revolution < config.revolution_count.value; ++revolution) {
        for (std::size_t tooth = 0; tooth < pulses_per_revolution; ++tooth) {
            // Attach the long gap to the first emitted pulse of each revolution so the
            // revolution boundary is visible in the generated timestamp sequence.
            const auto interval = (tooth == 0U) ? missing_tooth_gap : tooth_interval;
            current_timestamp += interval;

            pulses.push_back(PulseEvent{
                .timestamp = current_timestamp,
                .revolution_index = revolution,
                .tooth_index = tooth,
            });
        }
    }

    return pulses;
}

} // namespace vtcn::crank
