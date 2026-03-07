#include "vtcn/config/phase0_runtime.hpp"

#include <ostream>

namespace {

constexpr std::string_view kDaemonName = "vtcn-daemon";
constexpr std::string_view kRuntimeMode = "virtual-development";
constexpr std::string_view kProtocolTrack = "telemetry-v1-planning";

void write_help(std::ostream& out) {
    out << "vtcn-daemon Phase 0 placeholder\n";
    out << "mode: " << kRuntimeMode << '\n';
    out << "usage: vtcn-daemon [--help|--version]\n";
}

}  // namespace

namespace vtcn::config {

Phase0AppInfo phase0_app_info() {
    return {kDaemonName, kRuntimeMode, kProtocolTrack};
}

int run_phase0_cli(std::ostream& out,
                   std::ostream& err,
                   const std::vector<std::string_view>& args) {
    if (args.size() <= 1 || args[1] == "--help") {
        write_help(out);
        return 0;
    }

    if (args[1] == "--version") {
        const auto app_info = phase0_app_info();
        out << app_info.daemon_name << " phase0 " << app_info.runtime_mode << '\n';
        return 0;
    }

    err << "unknown argument: " << args[1] << '\n';
    return 1;
}

}  // namespace vtcn::config
