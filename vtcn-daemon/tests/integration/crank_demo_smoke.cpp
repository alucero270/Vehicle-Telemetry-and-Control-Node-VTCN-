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

    return 0;
}
