#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// GETS CURRENT GAME LANGUAGE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
TranslationsManager::Languages TranslationsManager::GetCurrentGameLanguage() {

	const char* currentPlatform = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::CurrentGamePlatform);

	std::string pathToSkyrimINIFile = std::format("{}\\{}\\Skyrim.ini", YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::PathToMyGames), currentPlatform);

	LogManager::Log(LogManager::LogLevel::Info, std::format("Getting \"Skyrim.ini\" file for language detection : \"{}\"", pathToSkyrimINIFile), false);

	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadFile(pathToSkyrimINIFile.c_str());

	if(rc != SI_OK) {

		LogManager::Log(LogManager::LogLevel::Error, std::format("Unable to find \"Skyrim.ini\" file in : \"{}\"", pathToSkyrimINIFile), false);
		LogManager::Log(LogManager::LogLevel::Error, "Message : Setting fallback translations to \"English\"", true);
		return Languages::English;

	};

	const char* language = ini.GetValue("General", "sLanguage", "ENGLISH");

	auto iterator = languageMap.find(language);

	if(iterator != languageMap.end()) { return iterator->second; };

	return Languages::English;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// INITIALIZES TRANSLATIONs MANAGER
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool TranslationsManager::Init() {

	TranslationsManager::Languages currentLocale = GetCurrentGameLanguage();

	switch(currentLocale) {

		case TranslationsManager::Languages::Chinese : {

			YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, "chinese");
			LogManager::Log(LogManager::LogLevel::Info, std::format("Detected language : \"{}\"", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale)), false);
			return true;

		};

		case TranslationsManager::Languages::English : {

			YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, "english");
			LogManager::Log(LogManager::LogLevel::Info, std::format("Detected language : \"{}\"", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale)), false);
			return true;

		};

		case TranslationsManager::Languages::French : {

			YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, "french");
			LogManager::Log(LogManager::LogLevel::Info, std::format("Detected language : \"{}\"", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale)), false);
			return true;

		};

		case TranslationsManager::Languages::German : {

			YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, "german");
			LogManager::Log(LogManager::LogLevel::Info, std::format("Detected language : \"{}\"", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale)), false);
			return true;

		};

		case TranslationsManager::Languages::Italian : {

			YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, "italian");
			LogManager::Log(LogManager::LogLevel::Info, std::format("Detected language : \"{}\"", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale)), false);
			return true;

		};

		case TranslationsManager::Languages::Japanese : {

			YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, "japanese");
			LogManager::Log(LogManager::LogLevel::Info, std::format("Detected language : \"{}\"", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale)), false);
			return true;

		};

		case TranslationsManager::Languages::Polish : {

			YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, "polish");
			LogManager::Log(LogManager::LogLevel::Info, std::format("Detected language : \"{}\"", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale)), false);
			return true;

		};

		case TranslationsManager::Languages::Russian : {

			YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, "russian");
			LogManager::Log(LogManager::LogLevel::Info, std::format("Detected language : \"{}\"", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale)), false);
			return true;

		};

	};

	return false;

};