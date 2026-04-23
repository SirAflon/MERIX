#pragma once
#include <thread>
#include <algorithm>

struct Settings{
    bool showStatus = false;
    bool ignorWarning = false;

    unsigned int maxThreads = std::max(1u, std::thread::hardware_concurrency() - 1);
};