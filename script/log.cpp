#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// ADDS A LINE BREAK IN LOG FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void LogManager::BreakLine() {

    auto logger = spdlog::get("log");
    logger->set_pattern("");
    logger->info("");
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%s:%#] %v");

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// CREATES LOG FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool LogManager::CreateLogFile() {

    std::string pathToSKSE = GetGlobal<std::string>(YGGDRASIL::Global::PathToSKSE);
    std::string pluginName = GetGlobal<const char*>(YGGDRASIL::Global::PluginName);

    if(!fs::exists(pathToSKSE)) return false;
    if(!fs::is_directory(pathToSKSE)) return false;

    YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToLogFile, std::format("{}\\{}.log", pathToSKSE, pluginName));

    std::string pathToLogFile = GetGlobal<std::string>(YGGDRASIL::Global::PathToLogFile);

    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(pathToLogFile, true);
    auto sink = std::make_shared<spdlog::logger>("log", std::move(fileSink));

    spdlog::set_default_logger(std::move(sink));
    return true;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// INITIALIZES LOG MANAGER
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool LogManager::Init() {

    PWSTR myDocuments = NULL;

    if(SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &myDocuments))) {

        char myDocumentsFolderPath[MAX_PATH];

        size_t convertedChars = 0;
        errno_t result = wcstombs_s(&convertedChars, myDocumentsFolderPath, sizeof(myDocumentsFolderPath), myDocuments, _TRUNCATE);

        if(result == 0) std::cout << "Conversion successful : " << myDocumentsFolderPath << "\n";

        if(!myDocumentsFolderPath) SKSE::stl::report_and_fail("Documents path not found, logs disabled.");

        std::wstring wStringMyDocuments(myDocuments);
        std::string dStringMyDocuments(wStringMyDocuments.begin(), wStringMyDocuments.end());

        YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToMyDocuments, myDocuments);
        YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToMyGames, std::format("{}\\My Games", dStringMyDocuments));

        CoTaskMemFree(myDocuments);

        bool foundGOG = YGGDRASIL::FindPlatform(YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::SkyrimGOG));
        bool foundSteam = YGGDRASIL::FindPlatform(YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::SkyrimSteam));

        if(foundGOG) {

            if(CreateLogFile()) {

                YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::SkyrimGOG));
                return true;

            };

        };

        if(foundSteam) {

            if(CreateLogFile()) {

                YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::SkyrimSteam));
                return true;

            };

        };

        if(!foundGOG && !foundSteam) return false;

    };

    return false;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// SWITCHES LOG
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void LogManager::Log(LogLevel type, const std::string& message, bool shouldBreakLine) {

    switch(type) {

        case LogLevel::Critical : {

            logger::critical("{}.", message);
            break;

        };

        case LogLevel::Debug : {

            logger::debug("{}.", message);
            break;

        };

        case LogLevel::Error : {

            logger::error("{}.", message);
            break;

        };

        case LogLevel::Info : {

            logger::info("{}.", message);
            break;

        };

        case LogLevel::Trace : {

            logger::trace("{}.", message);
            break;

        };

        case LogLevel::Warn : {

            logger::warn("{}.", message);
            break;

        };

    };

    if(shouldBreakLine) BreakLine();
    return;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// SETS LOG LEVEL
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void LogManager::SetLogLevel() {

    std::string pathToConfigurationFile = YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::PathToConfigurationFile);

    CSimpleIniA configuration = &ConfigurationManager::GetSingleton();

    SI_Error configurationLoadState = configuration.LoadFile(pathToConfigurationFile.c_str());

    if(configurationLoadState == 0) std::cout << "Configuration file loaded" << "\n";

    const char* defaultLogLevel = "Trace";
    const char* userLogLevelPreference = configuration.GetValue("DEBUG", "SetLogLevel", defaultLogLevel);

    spdlog::level::level_enum logLevel;

    if (strcmp(userLogLevelPreference, "Critical") == 0) {

        logLevel = spdlog::level::critical;

    } else if (strcmp(userLogLevelPreference, "Debug") == 0) {

        logLevel = spdlog::level::debug;

    } else if (strcmp(userLogLevelPreference, "Error") == 0) {

        logLevel = spdlog::level::err;

    } else if (strcmp(userLogLevelPreference, "Info") == 0) {

        logLevel = spdlog::level::info;

    } else if (strcmp(userLogLevelPreference, "Trace") == 0) {

        logLevel = spdlog::level::trace;

    } else if (strcmp(userLogLevelPreference, "Warn") == 0) {

        logLevel = spdlog::level::warn;

    } else {

        Log(LogLevel::Error, std::format("Unknown log level \"{}\" in configuration", userLogLevelPreference), false);
        Log(LogLevel::Error, std::format("Setting log level to : \"{}\"", defaultLogLevel), true);
        logLevel = spdlog::level::trace;

        spdlog::set_level(logLevel);
        spdlog::flush_on(logLevel);
        return;

    };

    Log(LogLevel::Info, "Loading log level from configuration", false);
    Log(LogLevel::Info, std::format("Setting log level to : \"{}\"", userLogLevelPreference), true);

    spdlog::set_level(logLevel);
    spdlog::flush_on(logLevel);

};