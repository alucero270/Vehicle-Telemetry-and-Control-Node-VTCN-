#pragma once

#include <iosfwd>
#include <string_view>
#include <vector>

namespace vtcn::config {

struct Phase0AppInfo {
    std::string_view daemon_name;
    std::string_view runtime_mode;
    std::string_view protocol_track;
};

Phase0AppInfo phase0_app_info();

int run_phase0_cli(std::ostream &out, std::ostream &err, const std::vector<std::string_view> &args);

} // namespace vtcn::config
