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

TEST(CrankDemoRuntimeTest, RuntimeSummaryMatchesDeterministicPipeline) {
    const auto summary = vtcn::runtime::crank_demo_runtime_summary();

    EXPECT_EQ(summary.profile.tooth_positions_per_revolution, 36U);
    EXPECT_EQ(summary.profile.missing_tooth_count, 1U);
    EXPECT_EQ(summary.target_rpm.value, 600U);
    EXPECT_EQ(summary.revolution_count.value, 2U);
    EXPECT_EQ(summary.pulses_generated, 70U);

    ASSERT_TRUE(summary.gap_result.has_value());
    EXPECT_EQ(summary.gap_result->gap_index, 35U);
    EXPECT_EQ(summary.gap_result->gap_interval.count(), 5'554);

    ASSERT_TRUE(summary.rpm_estimate.has_value());
    EXPECT_EQ(summary.rpm_estimate->estimated_rpm.value, 600U);
    EXPECT_EQ(summary.rpm_estimate->nominal_tooth_interval.count(), 2'777);
}

TEST(CrankDemoRuntimeTest, HelpTextDocumentsDemoArgumentAndValidationMode) {
    std::ostringstream out;
    std::ostringstream err;

    const auto exit_code = vtcn::runtime::run_crank_demo_cli(
        out, err, std::vector<std::string_view>{"vtcn-daemon", "--help"});

    EXPECT_EQ(exit_code, 0);
    EXPECT_TRUE(err.str().empty());
    EXPECT_NE(out.str().find("host-side Phase 0 validation"), std::string::npos);
    EXPECT_NE(out.str().find("--demo"), std::string::npos);
}

TEST(CrankDemoRuntimeTest, DefaultInvocationRunsDeterministicDemo) {
    std::ostringstream out;
    std::ostringstream err;

    const auto exit_code =
        vtcn::runtime::run_crank_demo_cli(out, err, std::vector<std::string_view>{"vtcn-daemon"});

    EXPECT_EQ(exit_code, 0);
    EXPECT_TRUE(err.str().empty());
    EXPECT_NE(out.str().find("target-rpm: 600"), std::string::npos);
    EXPECT_NE(out.str().find("estimated-rpm: 600"), std::string::npos);
    EXPECT_NE(out.str().find("gap-index: 35"), std::string::npos);
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
