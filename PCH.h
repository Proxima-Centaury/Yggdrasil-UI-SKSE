#pragma once

#include <algorithm>
#include <windows.h>
#include <shlobj.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <any>

#include "dependencies/SimpleINI/SimpleIni.h" // Dependency : manual integration

#include "RE/Skyrim.h" // Dependency : vcpkg integration
#include "SKSE/SKSE.h" // Dependency : vcpkg integration

#include <AL/al.h> // Dependency : vcpkg integration
#include <AL/alc.h> // Dependency : vcpkg integration

#include "include/yggdrasil.h"

#include "include/configuration.h"
#include "include/log.h"
#include "include/ui.h"
#include "include/sfx.h"

#include "include/menu/shared.h"

#include <spdlog/sinks/basic_file_sink.h>

using namespace std::literals;

namespace fs = std::filesystem;
namespace logger = SKSE::log;