#pragma once

#include "vtcn/crank/CrankTypes.hpp"

#include <iosfwd>
#include <string_view>
#include <vector>

namespace vtcn::runtime {

struct CrankDemoAppInfo final {
    std::string_view daemon_name;
    std::string_view runtime_mode;
    std::string_view protocol_track;
};

[[nodiscard]] CrankDemoAppInfo crank_demo_app_info() noexcept;
[[nodiscard]] crank::RuntimeSummary crank_demo_runtime_summary();

[[nodiscard]] int run_crank_demo_cli(std::ostream &out, std::ostream &err,
                                     const std::vector<std::string_view> &args);

} // namespace vtcn::runtime
