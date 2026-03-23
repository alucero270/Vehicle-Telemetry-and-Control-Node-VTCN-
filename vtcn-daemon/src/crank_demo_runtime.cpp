#include "vtcn/runtime/CrankDemoRuntime.hpp"

#include "vtcn/crank/GapDetector.hpp"
#include "vtcn/crank/RpmEstimator.hpp"
#include "vtcn/crank/SimulatedCrankPulseSource.hpp"

#include <cstddef>
#include <cstdint>
#include <ostream>

namespace {

constexpr std::string_view kDaemonName = "vtcn-daemon";
constexpr std::string_view kRuntimeMode = "host-side-validation";
constexpr std::string_view kProtocolTrack = "telemetry-v1-planning";
constexpr std::uint32_t kDemoTargetRpm = 600U;
constexpr std::size_t kDemoRevolutionCount = 2U;

[[nodiscard]] auto make_demo_config() -> vtcn::crank::CrankSignalConfig {
    return vtcn::crank::CrankSignalConfig{
        .target_rpm = vtcn::crank::Rpm{kDemoTargetRpm},
        .revolution_count = vtcn::crank::Revolutions{kDemoRevolutionCount},
        .start_time = vtcn::crank::Timestamp{0},
    };
}

void write_help(std::ostream &out) {
    out << "vtcn-daemon crank demo\n";
    out << "mode: " << kRuntimeMode << '\n';
    out << "purpose: host-side Phase 0 validation of the 36-1 crank pipeline\n";
    out << "usage: vtcn-daemon [--help|--version|--demo]\n";
    out << "  --demo    run deterministic source, gap detection, and RPM estimation\n";
}

void write_runtime_summary(std::ostream &out, const vtcn::crank::RuntimeSummary &summary) {
    out << "vtcn-daemon crank demo\n";
    out << "mode: " << kRuntimeMode << '\n';
    out << "validation: host-side Phase 0 deterministic 36-1 pipeline\n";
    out << "target-rpm: " << summary.target_rpm.value << '\n';
    out << "pulses-generated: " << summary.pulses_generated << '\n';

    if (summary.gap_result.has_value()) {
        out << "gap-index: " << summary.gap_result->gap_index << '\n';
        out << "gap-interval-us: " << summary.gap_result->gap_interval.count() << '\n';
    } else {
        out << "gap-detection: unavailable\n";
    }

    if (summary.rpm_estimate.has_value()) {
        out << "estimated-rpm: " << summary.rpm_estimate->estimated_rpm.value << '\n';
        out << "nominal-tooth-interval-us: "
            << summary.rpm_estimate->nominal_tooth_interval.count() << '\n';
    } else {
        out << "estimated-rpm: unavailable\n";
    }
}

} // namespace

namespace vtcn::runtime {

CrankDemoAppInfo crank_demo_app_info() noexcept {
    return {kDaemonName, kRuntimeMode, kProtocolTrack};
}

crank::RuntimeSummary crank_demo_runtime_summary() {
    const auto profile = crank::default_crank_wheel_profile();
    const auto config = make_demo_config();

    const crank::SimulatedCrankPulseSource source{profile};
    const auto pulses = source.generate(config);

    const crank::GapDetector detector{};
    const auto gap_result = detector.detect(pulses);

    const crank::RpmEstimator estimator{};
    const auto rpm_estimate =
        gap_result.has_value() ? estimator.estimate(pulses, *gap_result) : std::nullopt;

    return crank::RuntimeSummary{
        .profile = profile,
        .target_rpm = config.target_rpm,
        .revolution_count = config.revolution_count,
        .pulses_generated = pulses.size(),
        .gap_result = gap_result,
        .rpm_estimate = rpm_estimate,
    };
}

int run_crank_demo_cli(std::ostream &out, std::ostream &err,
                       const std::vector<std::string_view> &args) {
    if (args.size() > 1 && args[1] == "--help") {
        write_help(out);
        return 0;
    }

    if (args.size() > 1 && args[1] == "--version") {
        const auto app_info = crank_demo_app_info();
        out << app_info.daemon_name << " crank-demo " << app_info.runtime_mode << '\n';
        return 0;
    }

    if (args.size() <= 1 || args[1] == "--demo") {
        write_runtime_summary(out, crank_demo_runtime_summary());
        return 0;
    }

    err << "unknown argument: " << args[1] << '\n';
    return 1;
}

} // namespace vtcn::runtime
