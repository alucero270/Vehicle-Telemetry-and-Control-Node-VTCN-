#pragma once

#include "vtcn/crank/CrankTypes.hpp"

#include <span>

namespace vtcn::crank {

class GapDetector final {
  public:
    [[nodiscard]] std::optional<GapDetectionResult>
    detect(std::span<const PulseEvent> pulses) const;
};

} // namespace vtcn::crank
