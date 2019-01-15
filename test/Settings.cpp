#include "Settings.h"

// We need CMake to provide us the absolute path to the directory containing
// test files, so we are certain to find those files no matter where the test
// harness binary is generated. This provides out-of-source build capability.
// This will be used as the default test root, but can be overridden with
// the --test-root argument.
#ifndef SLANG_TEST_DIRECTORY
#error \
    "SLANG_TEST_DIRECTORY needs to be defined for gtest to locate test files."
#endif

GTestSettings GlobalTestSettings = {SLANG_TEST_DIRECTORY};
