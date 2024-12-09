#include <algorithm>
#include <windows.h>
#include <shlobj.h>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <any>

#include "RE/Skyrim.h" // Dependency : vcpkg integration
#include "SKSE/SKSE.h" // Dependency : vcpkg integration

#include "include/yggdrasil.h"
#include "include/log.h"

#include <spdlog/sinks/basic_file_sink.h>

using namespace std::literals;

namespace fs = std::filesystem;
namespace logger = SKSE::log;