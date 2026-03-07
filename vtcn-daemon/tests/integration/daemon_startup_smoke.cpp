#include "vtcn/config/phase0_runtime.hpp"

#include <sstream>
#include <string>
#include <vector>

int main() {
    const auto app_info = vtcn::config::phase0_app_info();
    if (app_info.daemon_name != "vtcn-daemon") {
        return 1;
    }

    if (app_info.runtime_mode != "virtual-development") {
        return 1;
    }

    std::ostringstream out;
    std::ostringstream err;

    const auto exit_code =
        vtcn::config::run_phase0_cli(
            out,
            err,
            std::vector<std::string_view>{"vtcn-daemon"});

    if (exit_code != 0) {
        return 1;
    }

    if (!err.str().empty()) {
        return 1;
    }

    if (out.str().find("vtcn-daemon") == std::string::npos) {
        return 1;
    }

    if (out.str().find("virtual-development") == std::string::npos) {
        return 1;
    }

    return 0;
}
