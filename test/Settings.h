#pragma once

#include <string>

struct GTestSettings {
    // The root directory for test files.
    std::string testRoot;
};

extern GTestSettings GlobalTestSettings;
