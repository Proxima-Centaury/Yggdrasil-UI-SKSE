#pragma once

#include "RE/Skyrim.h" // Dependency : vcpkg integration
#include "SKSE/SKSE.h" // Dependency : vcpkg integration

#include <algorithm>
#include <windows.h>
#include <shlobj.h>
#include <stdlib.h>
#include <stdio.h>

#include <spdlog/sinks/basic_file_sink.h>

using namespace std::literals;

namespace fs = std::filesystem;
namespace logger = SKSE::log;