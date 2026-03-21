#include "vtcn/crank/GapDetector.hpp"

namespace vtcn::crank {

std::optional<GapDetectionResult> GapDetector::detect(std::span<const PulseEvent> pulses) const {
    (void)pulses;
    return std::nullopt;
}

} // namespace vtcn::crank
