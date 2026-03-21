#pragma once

#include "vtcn/crank/ICrankPulseSource.hpp"

namespace vtcn::crank {

class SimulatedCrankPulseSource final : public ICrankPulseSource {
  public:
    explicit SimulatedCrankPulseSource(
        CrankWheelProfile profile = default_crank_wheel_profile()) noexcept;

    [[nodiscard]] const CrankWheelProfile &profile() const noexcept;

    [[nodiscard]] std::vector<PulseEvent> generate(const CrankSignalConfig &config) const override;

  private:
    CrankWheelProfile profile_;
};

} // namespace vtcn::crank
