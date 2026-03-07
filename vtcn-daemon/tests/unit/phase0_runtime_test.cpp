#include "vtcn/config/phase0_runtime.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

namespace {

TEST(Phase0RuntimeTest, AppInfoMatchesVirtualDevelopmentMode) {
    const auto app_info = vtcn::config::phase0_app_info();

    EXPECT_EQ(app_info.daemon_name, "vtcn-daemon");
    EXPECT_EQ(app_info.runtime_mode, "virtual-development");
    EXPECT_EQ(app_info.protocol_track, "telemetry-v1-planning");
}

TEST(Phase0RuntimeTest, HelpTextAdvertisesVirtualMode) {
    std::ostringstream out;
    std::ostringstream err;

    const auto exit_code =
        vtcn::config::run_phase0_cli(
            out,
            err,
            std::vector<std::string_view>{"vtcn-daemon", "--help"});

    EXPECT_EQ(exit_code, 0);
    EXPECT_TRUE(err.str().empty());
    EXPECT_NE(out.str().find("virtual-development"), std::string::npos);
}

TEST(Phase0RuntimeTest, UnknownArgumentFailsClearly) {
    std::ostringstream out;
    std::ostringstream err;

    const auto exit_code =
        vtcn::config::run_phase0_cli(
            out,
            err,
            std::vector<std::string_view>{"vtcn-daemon", "--unknown"});

    EXPECT_NE(exit_code, 0);
    EXPECT_TRUE(out.str().empty());
    EXPECT_NE(err.str().find("--unknown"), std::string::npos);
}

}  // namespace
