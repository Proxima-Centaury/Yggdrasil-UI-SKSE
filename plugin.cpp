#include "PCH.h"

#define DLLEXPORT __declspec(dllexport)

extern "C" DLLEXPORT bool SKSEPlugin_Load(const SKSE::LoadInterface* skse) {

	SKSE::Init(skse);

	YGGDRASIL::ToggleDebuggingConsole(true);

	bool isConfigurationInitialized = YGGDRASIL::Initialize(YGGDRASIL::Manager::Configuration);
	bool isLogInitialized = YGGDRASIL::Initialize(YGGDRASIL::Manager::Log);
	bool isTranslationInitialized = YGGDRASIL::Initialize(YGGDRASIL::Manager::Translation);

	auto runtimeVersion = skse->RuntimeVersion();
	auto scriptExtenderVersion = skse->SKSEVersion();

	uint32_t scriptExtenderMajor = (scriptExtenderVersion >> 24) & 0xFF;
	uint32_t scriptExtenderMinor = (scriptExtenderVersion >> 16) & 0xFF;
	uint32_t scriptExtenderPatch = scriptExtenderVersion & 0xFF;

	std::string skseVersion = std::format("{}.{}.{}", scriptExtenderMajor, scriptExtenderMinor, scriptExtenderPatch / 0x10);
	std::string tesvVersion = std::format("{}.{}.{}", runtimeVersion.major(), runtimeVersion.minor(), runtimeVersion.patch());
	std::string yguiVersion = "1.0.0";

	YGGDRASIL::SetGlobal(YGGDRASIL::Global::SKSEVersion, skseVersion);
	YGGDRASIL::SetGlobal(YGGDRASIL::Global::TESVVersion, tesvVersion);
	YGGDRASIL::SetGlobal(YGGDRASIL::Global::YGUIVersion, yguiVersion);

	if(isConfigurationInitialized && isLogInitialized && isTranslationInitialized) {

		LogManager::SetLogLevel();

		std::string pathToJSONFiles = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PathToJSONFiles, "PathToJSONFiles");
		std::string pathToLogFile = YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::PathToLogFile, "PathToLogFile");
		std::string pluginName = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PluginName, "PluginName");

		LogManager::Log(LogManager::LogLevel::Information, std::format("\"{}.json\" found in : \"{}\"", pluginName, pathToJSONFiles), false);
		LogManager::Log(LogManager::LogLevel::Information, std::format("\"{}.log\" found in : \"{}\"", pluginName, pathToLogFile), true);

		SKSE::GetMessagingInterface()->RegisterListener(YGGDRASIL::OnSKSEMessage);

		return true;

	} else {

		if(!isConfigurationInitialized) SKSE::stl::report_and_fail("Unable to load configuration, report to developer for further investigation");
		if(!isLogInitialized) SKSE::stl::report_and_fail("Unable to load logs, report to developer for further investigation");
		if(!isTranslationInitialized) SKSE::stl::report_and_fail("Unable to load translations, report to developer for further investigation");

		return false;

	};

};