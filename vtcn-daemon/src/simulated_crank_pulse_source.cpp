#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

namespace vtcn::crank {

SimulatedCrankPulseSource::SimulatedCrankPulseSource(CrankWheelProfile profile) noexcept
    : profile_(profile) {}

const CrankWheelProfile &SimulatedCrankPulseSource::profile() const noexcept {
    return profile_;
}

std::vector<PulseEvent> SimulatedCrankPulseSource::generate(const CrankSignalConfig &config) const {
    (void)config;
    return {};
}

} // namespace vtcn::crank
