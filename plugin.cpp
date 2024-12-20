#include "PCH.h"

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT bool SKSEPlugin_Load(const SKSE::LoadInterface* skse) {

    SKSE::Init(skse);

    YGGDRASIL::ToggleDebuggingConsole(true);

    bool isConfigurationInitialized = YGGDRASIL::Init(YGGDRASIL::Manager::Configuration);
    bool isLogInitialized = YGGDRASIL::Init(YGGDRASIL::Manager::Log);
    bool isTranslationInitialized = YGGDRASIL::Init(YGGDRASIL::Manager::Translation);

    auto extenderVersion = skse->SKSEVersion();
    auto runtimeVersion = skse->RuntimeVersion();

    uint32_t extenderMajor = (extenderVersion >> 24) & 0xFF;
    uint32_t extenderMinor = (extenderVersion >> 16) & 0xFF;
    uint32_t extenderPatch = extenderVersion & 0xFF;

    std::string gameVersion = std::format("{}.{}.{}", runtimeVersion.major(), runtimeVersion.minor(), runtimeVersion.patch());
    std::string skseVersion = std::format("{}.{}.{}", extenderMajor, extenderMinor, extenderPatch / 0x10);
    std::string uiVersion = "1.0.0";

    YGGDRASIL::SetGlobal(YGGDRASIL::Global::GAMEVersion, gameVersion);
    YGGDRASIL::SetGlobal(YGGDRASIL::Global::SKSEVersion, skseVersion);
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