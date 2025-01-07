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

	std::string pathToSKSE = GetGlobal<std::string>(YGGDRASIL::Global::PathToSKSE, "PathToSKSE");
	std::string pluginName = GetGlobal<const char*>(YGGDRASIL::Global::PluginName, "PluginName");

	if(!fs::exists(pathToSKSE)) return false;
	if(!fs::is_directory(pathToSKSE)) return false;

	YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToLogFile, std::format("{}\\{}.log", pathToSKSE, pluginName));

	std::string pathToLogFile = GetGlobal<std::string>(YGGDRASIL::Global::PathToLogFile, "PathToLogFile");

	auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(pathToLogFile, true);
	auto sink = std::make_shared<spdlog::logger>("log", std::move(fileSink));

	std::cout << "Log file created at : " << pathToLogFile << std::endl;

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

		if(result == 0) std::cout << "Conversion successful : " << myDocumentsFolderPath << std::endl;

		if(!myDocumentsFolderPath) SKSE::stl::report_and_fail("My Documents path not found.");

		std::wstring wStringMyDocuments(myDocuments);
		std::string dStringMyDocuments(wStringMyDocuments.begin(), wStringMyDocuments.end());

		YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToMyDocuments, myDocuments);
		YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToMyGames, std::format("{}\\My Games", dStringMyDocuments));

		CoTaskMemFree(myDocuments);

		std::string skyrimGOG = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::SkyrimGOG, "SkyrimGOG");
		std::string skyrimSteam = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::SkyrimSteam, "SkyrimSteam");

		bool foundGOG = YGGDRASIL::FindGamePlatform(skyrimGOG.c_str());
		bool foundSteam = YGGDRASIL::FindGamePlatform(skyrimSteam.c_str());

		if(foundGOG) {

			if(CreateLogFile()) {

				YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentGamePlatform, skyrimGOG);
				return true;

			};

		};

		if(foundSteam) {

			if(CreateLogFile()) {

				YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentGamePlatform, skyrimSteam);
				return true;

			};

		};

		if(!foundGOG && !foundSteam) {

			std::cout << "Failed to create log file" << std::endl;
			return false;

		};

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

		case LogLevel::Information : {

			logger::info("{}.", message);
			break;

		};

		case LogLevel::Trace : {

			logger::trace("{}.", message);
			break;

		};

		case LogLevel::Warning : {

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

	json configuration = YGGDRASIL::GetGlobal<json>(YGGDRASIL::Global::Configuration, "Configuration");

	std::string level = configuration["debug"];
	std::string defaultLevel = "Trace";

	spdlog::level::level_enum logLevel;

	if (strcmp(level.c_str(), "Critical") == 0) {

		logLevel = spdlog::level::critical;

	} else if (strcmp(level.c_str(), "Debug") == 0) {

		logLevel = spdlog::level::debug;

	} else if (strcmp(level.c_str(), "Error") == 0) {

		logLevel = spdlog::level::err;

	} else if (strcmp(level.c_str(), "Information") == 0) {

		logLevel = spdlog::level::info;

	} else if (strcmp(level.c_str(), "Trace") == 0) {

		logLevel = spdlog::level::trace;

	} else if (strcmp(level.c_str(), "Warning") == 0) {

		logLevel = spdlog::level::warn;

	} else {

		Log(LogLevel::Warning, std::format("Unknown log level \"{}\" in configuration", level), false);
		Log(LogLevel::Warning, std::format("Setting log level to : \"{}\"", defaultLevel), true);
		logLevel = spdlog::level::trace;

		spdlog::set_level(logLevel);
		spdlog::flush_on(logLevel);
		return;

	};

	Log(LogLevel::Information, "Loading log level from configuration", false);
	Log(LogLevel::Information, std::format("Setting log level to : \"{}\"", level), true);

	spdlog::set_level(logLevel);
	spdlog::flush_on(logLevel);

};