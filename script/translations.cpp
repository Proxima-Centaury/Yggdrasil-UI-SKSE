#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// GETS CURRENT GAME LANGUAGE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
TranslationsManager::Languages TranslationsManager::GetCurrentGameLanguage() {

	std::string currentPlatform = YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::CurrentGamePlatform, "CurrentGamePlatform");

	std::string pathToSkyrimINIFile = std::format("{}\\{}\\Skyrim.ini", YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::PathToMyGames, "PathToMyGames"), currentPlatform);

	LogManager::Log(LogManager::LogLevel::Information, std::format("Getting \"Skyrim.ini\" file for language detection : \"{}\"", pathToSkyrimINIFile), false);

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

	bool supportedLocale;

	std::string locale;

	TranslationsManager::Languages currentLocale = GetCurrentGameLanguage();

	switch(currentLocale) {

		case TranslationsManager::Languages::Chinese : {

			locale = "Chinese";
			supportedLocale = true;

		};

		case TranslationsManager::Languages::English : {

			locale = "English";
			supportedLocale = true;

		};

		case TranslationsManager::Languages::French : {

			locale = "French";
			supportedLocale = true;

		};

		case TranslationsManager::Languages::German : {

			locale = "German";
			supportedLocale = true;

		};

		case TranslationsManager::Languages::Italian : {

			locale = "Italian";
			supportedLocale = true;

		};

		case TranslationsManager::Languages::Japanese : {

			locale = "Japanese";
			supportedLocale = true;

		};

		case TranslationsManager::Languages::Polish : {

			locale = "Polish";
			supportedLocale = true;

		};

		case TranslationsManager::Languages::Russian : {

			locale = "Russian";
			supportedLocale = true;

		};

		default : {

			supportedLocale = false;

		};

	};

	if(supportedLocale) {

		YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentLocale, locale);
		LogManager::Log(LogManager::LogLevel::Information, std::format("Detected language : \"{}\"", locale), false);
		LogManager::Log(LogManager::LogLevel::Information, std::format("\"{}\" language is supported by Yggdrasil UI", locale), true);

	} else {

		LogManager::Log(LogManager::LogLevel::Warning, "Detected an unsupported language", false);
		LogManager::Log(LogManager::LogLevel::Warning, "Check your Skyrim.ini files and set a supported \"sLanguage\" value under \"[General]\" section", true);

	};

	return supportedLocale;

};