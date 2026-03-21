#include "vtcn/runtime/CrankDemoRuntime.hpp"

#include <ostream>

namespace {

constexpr std::string_view kDaemonName = "vtcn-daemon";
constexpr std::string_view kRuntimeMode = "host-side-validation";
constexpr std::string_view kProtocolTrack = "telemetry-v1-planning";

void write_help(std::ostream &out) {
    out << "vtcn-daemon crank demo placeholder\n";
    out << "mode: " << kRuntimeMode << '\n';
    out << "usage: vtcn-daemon [--help|--version]\n";
}

} // namespace

namespace vtcn::runtime {

CrankDemoAppInfo crank_demo_app_info() noexcept {
    return {kDaemonName, kRuntimeMode, kProtocolTrack};
}

int run_crank_demo_cli(std::ostream &out, std::ostream &err,
                       const std::vector<std::string_view> &args) {
    if (args.size() <= 1 || args[1] == "--help") {
        write_help(out);
        return 0;
    }

    if (args[1] == "--version") {
        const auto app_info = crank_demo_app_info();
        out << app_info.daemon_name << " crank-demo " << app_info.runtime_mode << '\n';
        return 0;
    }

    err << "unknown argument: " << args[1] << '\n';
    return 1;
}

} // namespace vtcn::runtime
