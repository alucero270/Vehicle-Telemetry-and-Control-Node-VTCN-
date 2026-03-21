#include "vtcn/runtime/CrankDemoRuntime.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

namespace {

TEST(CrankDemoRuntimeTest, AppInfoMatchesHostSideValidationMode) {
    const auto app_info = vtcn::runtime::crank_demo_app_info();

    EXPECT_EQ(app_info.daemon_name, "vtcn-daemon");
    EXPECT_EQ(app_info.runtime_mode, "host-side-validation");
    EXPECT_EQ(app_info.protocol_track, "telemetry-v1-planning");
}

TEST(CrankDemoRuntimeTest, HelpTextAdvertisesHostSideValidation) {
    std::ostringstream out;
    std::ostringstream err;

    const auto exit_code = vtcn::runtime::run_crank_demo_cli(
        out, err, std::vector<std::string_view>{"vtcn-daemon", "--help"});

    EXPECT_EQ(exit_code, 0);
    EXPECT_TRUE(err.str().empty());
    EXPECT_NE(out.str().find("host-side-validation"), std::string::npos);
}

TEST(CrankDemoRuntimeTest, UnknownArgumentFailsClearly) {
    std::ostringstream out;
    std::ostringstream err;

    const auto exit_code = vtcn::runtime::run_crank_demo_cli(
        out, err, std::vector<std::string_view>{"vtcn-daemon", "--unknown"});

    EXPECT_NE(exit_code, 0);
    EXPECT_TRUE(out.str().empty());
    EXPECT_NE(err.str().find("--unknown"), std::string::npos);
}

} // namespace
