#include "vtcn/runtime/CrankDemoRuntime.hpp"

#include <iostream>
#include <string_view>
#include <vector>

int main(int argc, char *argv[]) {
    std::vector<std::string_view> args;
    args.reserve(static_cast<std::size_t>(argc));

    for (int index = 0; index < argc; ++index) {
        args.emplace_back(argv[index]);
    }

    return vtcn::runtime::run_crank_demo_cli(std::cout, std::cerr, args);
}
