add_library(vtcn_warnings INTERFACE)

# Keep warning policy scoped to active VTCN targets rather than third-party dependencies.
target_compile_options(vtcn_warnings
    INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra -Wpedantic>
)

add_library(vtcn_daemon_phase0 STATIC
    src/phase0_runtime.cpp
)

target_include_directories(vtcn_daemon_phase0
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(vtcn_daemon_phase0 PUBLIC cxx_std_23)

target_link_libraries(vtcn_daemon_phase0
    PUBLIC
        vtcn_warnings
)

add_executable(vtcn-daemon
    src/main.cpp
)

target_link_libraries(vtcn-daemon
    PRIVATE
        vtcn_daemon_phase0
)

if(BUILD_TESTING)
    find_package(GTest QUIET)

    if(NOT GTest_FOUND)
        include(FetchContent)

        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )

        FetchContent_MakeAvailable(googletest)
    endif()

    add_executable(vtcn-daemon-unit-tests
        tests/unit/phase0_runtime_test.cpp
    )

    target_link_libraries(vtcn-daemon-unit-tests
        PRIVATE
            vtcn_daemon_phase0
            GTest::gtest_main
    )

    add_test(NAME vtcn-daemon-unit-tests COMMAND vtcn-daemon-unit-tests)

    add_executable(vtcn-daemon-integration-smoke
        tests/integration/daemon_startup_smoke.cpp
    )

    target_link_libraries(vtcn-daemon-integration-smoke
        PRIVATE
            vtcn_daemon_phase0
    )

    add_test(NAME vtcn-daemon-integration-smoke COMMAND vtcn-daemon-integration-smoke)
    add_test(NAME vtcn-daemon-cli-help COMMAND vtcn-daemon --help)

    set_tests_properties(vtcn-daemon-cli-help
        PROPERTIES
            PASS_REGULAR_EXPRESSION "virtual-development"
    )
endif()
