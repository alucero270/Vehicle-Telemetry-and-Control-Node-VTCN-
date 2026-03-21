#pragma once

#include "vtcn/crank/CrankTypes.hpp"

#include <vector>

namespace vtcn::crank {

class ICrankPulseSource {
  public:
    virtual ~ICrankPulseSource() = default;

    [[nodiscard]] virtual std::vector<PulseEvent>
    generate(const CrankSignalConfig &config) const = 0;
};

} // namespace vtcn::crank
