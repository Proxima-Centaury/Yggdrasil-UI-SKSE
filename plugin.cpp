#include "PCH.h"

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT bool SKSEPlugin_Load(const SKSE::LoadInterface* skse) {

    SKSE::Init(skse);

    YGGDRASIL::ToggleDebuggingConsole(false);

    bool isConfigurationInitialized = YGGDRASIL::Init(YGGDRASIL::Manager::Configuration);
    bool isLogInitialized = YGGDRASIL::Init(YGGDRASIL::Manager::Log);
    bool isTranslationInitialized = YGGDRASIL::Init(YGGDRASIL::Manager::Translation);

    auto extenderVersion = skse->SKSEVersion();
    auto runtimeVersion = skse->RuntimeVersion();
    
    std::string gameVersion = std::format("{}.{}.{}", runtimeVersion.major(), runtimeVersion.minor(), runtimeVersion.patch());
    std::string uiVersion = "1.0.0";

    YGGDRASIL::SetGlobal(YGGDRASIL::Global::GAMEVersion, gameVersion);
    YGGDRASIL::SetGlobal(YGGDRASIL::Global::SKSEVersion, extenderVersion);
    YGGDRASIL::SetGlobal(YGGDRASIL::Global::YGUIVersion, uiVersion);

    if(isConfigurationInitialized && isLogInitialized && isTranslationInitialized) {

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