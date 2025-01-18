#pragma once
#pragma comment(lib, "Xinput.lib")

#include <algorithm>
#include <any>
#include <atomic>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <optional>
#include <shlobj.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <windows.h>

#include <Xinput.h>

#include "RE/Skyrim.h" // Dependency : vcpkg integration
#include "SKSE/SKSE.h" // Dependency : vcpkg integration

#include "dependencies/SimpleINI/SimpleIni.h" // Dependency : manual integration

#include <nlohmann/json.hpp> // Dependency : vcpkg integration

#include <AL/al.h> // Dependency : vcpkg integration
#include <AL/alc.h> // Dependency : vcpkg integration

#include "include/yggdrasil.h"

#include "include/configuration.h"
#include "include/log.h"
#include "include/sfx.h"
#include "include/translations.h"
#include "include/ui.h"
#include "include/sfx.h"

#include "include/menu/shared.h"

#include <spdlog/sinks/basic_file_sink.h>

using json = nlohmann::json;
using namespace std::literals;

namespace fs = std::filesystem;
namespace logger = SKSE::log;