#include "../include/yggdrasil.h"

/* ------------------------------------------------------------------------------------------------------------------------------------------ */
/* CREATES LOG FILE */
/* ------------------------------------------------------------------------------------------------------------------------------------------ */
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

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* INITIALIZES LOG MANAGER */
/* --------------------------------------------------------------------------------------------------------------------------------- */
bool LogManager::Init() {

    PWSTR myDocuments = NULL;

    if(SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &myDocuments))) {

        char myDocumentsFolderPath[MAX_PATH];

        size_t convertedChars = 0;
        errno_t result = wcstombs_s(&convertedChars, myDocumentsFolderPath, sizeof(myDocumentsFolderPath), myDocuments, _TRUNCATE);

        if(!myDocumentsFolderPath) SKSE::stl::report_and_fail("Documents path not found, logs disabled.");

        std::wstring wStringMyDocuments(myDocuments);
        std::string dStringMyDocuments(wStringMyDocuments.begin(), wStringMyDocuments.end());

        YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToMyDocuments, myDocuments);
        YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToMyGames, std::format("{}\\My Games", dStringMyDocuments));

        CoTaskMemFree(myDocuments);

        bool foundGOG = YGGDRASIL::FindPlatform(YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::SkyrimGOG));
        bool foundSteam = YGGDRASIL::FindPlatform(YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::SkyrimSteam));

        if(foundGOG) {

            if(CreateLogFile()) return true;

        };

        if(foundSteam) {

            if(CreateLogFile()) return true;

        };

        if(!foundGOG && !foundSteam) return false;

    };

    return false;

};