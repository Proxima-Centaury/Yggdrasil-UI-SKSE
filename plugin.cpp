#include "PCH.h"

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT bool SKSEPlugin_Load(const SKSE::LoadInterface* skse) {

    SKSE::Init(skse);

    bool isConfigurationInitialized = YGGDRASIL::Init(YGGDRASIL::Manager::Configuration);
    bool isLogInitialized = YGGDRASIL::Init(YGGDRASIL::Manager::Log);

    if(isConfigurationInitialized && isLogInitialized) {

        LogManager::SetLogLevel();

        std::string pathToConfigurationFile = YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::PathToConfigurationFile);
        std::string pathToLogFile = YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::PathToLogFile);
        std::string pluginName = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PluginName);

        LogManager::Log(LogManager::LogLevel::Info, std::format("\"{}.ini\" found in : \"{}\"", pluginName, pathToConfigurationFile), false);
        LogManager::Log(LogManager::LogLevel::Info, std::format("\"{}.log\" found in : \"{}\"", pluginName, pathToLogFile), true);

        SKSE::GetMessagingInterface()->RegisterListener(YGGDRASIL::OnSKSEMessage);

    };

    return true;

};