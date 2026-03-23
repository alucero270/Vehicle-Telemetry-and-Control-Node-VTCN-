#include "vtcn/runtime/CrankDemoRuntime.hpp"

#include <sstream>
#include <string>
#include <vector>

int main() {
    const auto app_info = vtcn::runtime::crank_demo_app_info();
    if (app_info.daemon_name != "vtcn-daemon") {
        return 1;
    }

    if (app_info.runtime_mode != "host-side-validation") {
        return 1;
    }

    const auto summary = vtcn::runtime::crank_demo_runtime_summary();
    if (!summary.gap_result.has_value()) {
        return 1;
    }

    if (!summary.rpm_estimate.has_value()) {
        return 1;
    }

    if (summary.target_rpm.value != 600U) {
        return 1;
    }

    if (summary.rpm_estimate->estimated_rpm.value != 600U) {
        return 1;
    }

    std::ostringstream out;
    std::ostringstream err;

    const auto exit_code =
        vtcn::runtime::run_crank_demo_cli(out, err, std::vector<std::string_view>{"vtcn-daemon"});

    if (exit_code != 0) {
        return 1;
    }

    if (!err.str().empty()) {
        return 1;
    }

    if (out.str().find("vtcn-daemon") == std::string::npos) {
        return 1;
    }

    if (out.str().find("host-side-validation") == std::string::npos) {
        return 1;
    }

    if (out.str().find("target-rpm: 600") == std::string::npos) {
        return 1;
    }

    if (out.str().find("estimated-rpm: 600") == std::string::npos) {
        return 1;
    }

    if (out.str().find("gap-index: 35") == std::string::npos) {
        return 1;
    }

    return 0;
}
