#pragma once

#include "vtcn/crank/CrankTypes.hpp"

#include <span>

namespace vtcn::crank {

class RpmEstimator final {
  public:
    [[nodiscard]] std::optional<RpmEstimate> estimate(std::span<const PulseEvent> pulses,
                                                      const GapDetectionResult &gap_result) const;
};

} // namespace vtcn::crank
