#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// GETS CURRENT GAME LANGUAGE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
TranslationsManager::Languages TranslationsManager::GetCurrentGameLanguage() {

    const char* currentPlatform = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::CurrentPlatform);

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
// GETS FILE NAME ACCORDING TO GAME LANGUAGE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
std::string TranslationsManager::GetSkyrimTranslationsFileName() {

    Languages currentLanguage = GetCurrentGameLanguage();

    auto iterator = languageFiles.find(currentLanguage);

    if(iterator != languageFiles.end()) { return iterator->second; };

    return "translate_english.txt";

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// INITIALIZES TRANSLATIONs MANAGER
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool TranslationsManager::Init() {

    std::string skyrimTranslationsFileName = GetSkyrimTranslationsFileName();

    if(skyrimTranslationsFileName.starts_with("translate_") && skyrimTranslationsFileName.ends_with(".txt")) {

        const std::string language = skyrimTranslationsFileName.substr(10, skyrimTranslationsFileName.size() - 14);

        LogManager::Log(LogManager::LogLevel::Info, std::format("Getting \"{}\" vanilla translation file", skyrimTranslationsFileName), false);
        LogManager::Log(LogManager::LogLevel::Info, std::format("Extracting language from file name : \"{}\"", language), false);

        const std::string customTranslationsFilePath = std::format("{}\\{}.txt", YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PathToUITranslationsFiles), language);

        LogManager::Log(LogManager::LogLevel::Info, std::format("Reading \"Yggdrasil UI\" translations from : \"{}\"", customTranslationsFilePath), false);

        if(fs::exists(customTranslationsFilePath) && ReadTranslationsFile(customTranslationsFilePath)) {

            return WriteInTranslationsFile(skyrimTranslationsFileName);

        };

        return false;

    };

    return false;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// READS EXISTING TRANSLATION KEYS FROM VANILLA GAME TRANSLATIONS FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool TranslationsManager::ReadExistingTranslationKeys(const std::string& filePath) {

    std::wifstream file(filePath, std::ios::binary);

    if(!file.is_open()) {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Failed to open translation file : \"{}\"", filePath), true);
        return false;

    };

    file.imbue(std::locale(file.getloc(), new std::codecvt_utf16<wchar_t, 0x10FFFF, std::little_endian>));

    std::wstring line;

    while(std::getline(file, line)) {

        size_t separatorPosition = line.find(L'\t');

        if(separatorPosition != std::string::npos) {

            std::wstring wideKey = line.substr(0, separatorPosition);

            std::string key = YGGDRASIL::UTF16ToUTF8(wideKey);

            existingKeys.insert(key);

        };

    };

    file.close();
    return true;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// READS TRANSLATIONs FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool TranslationsManager::ReadTranslationsFile(const std::string& filePath) {

    std::ifstream file(filePath);

    if(!file.is_open()) {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Failed to open translation file : \"{}\"", filePath), true);
        return false;

    };

    std::string line;

    while(std::getline(file, line)) {

        size_t separatorPosition = line.find("=");

        if(separatorPosition != std::string::npos) {

            std::string key = line.substr(0, separatorPosition);
            std::string value = line.substr(separatorPosition + 1);

            YGGDRASIL::TrimLeadingSpaces(key);
            YGGDRASIL::TrimLeadingSpaces(value);

            translations[key] = value;

        };

    };

    file.close();
    return true;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// WRITES IN BASE GAME'S TRANSLATIONs FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool TranslationsManager::WriteInTranslationsFile(const std::string& skyrimTranslationsFileName) {

    std::string skyrimTranslationsFilePath = std::format("{}\\{}", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::PathToSkyrimInterface), skyrimTranslationsFileName);

    // ReadExistingTranslationKeys(skyrimTranslationsFilePath);

    // Append mode
    // std::ofstream file(skyrimTranslationsFilePath, std::ios::binary | std::ios::app);
    // Rewrite mode
    std::ofstream file(skyrimTranslationsFilePath, std::ios::binary | std::ios::trunc);

    if(!file.is_open()) {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Failed to open translation file : \"{}\"", skyrimTranslationsFilePath), true);
        return false;

    };

    for(const auto& [key, value] : translations) {

        std::wstring wideKey = YGGDRASIL::UTF8ToUTF16(key);
        std::wstring wideValue = YGGDRASIL::UTF8ToUTF16(value);

        // if(existingKeys.find(key) != existingKeys.end()) continue;

        file.write(reinterpret_cast<const char*>(wideKey.c_str()), wideKey.size() * sizeof(wchar_t));

        wchar_t separator = L'\t';

        file.write(reinterpret_cast<const char*>(&separator), sizeof(wchar_t));
        file.write(reinterpret_cast<const char*>(wideValue.c_str()), wideValue.size() * sizeof(wchar_t));

        wchar_t newline[] = L"\r\n";

        file.write(reinterpret_cast<const char*>(newline), 2 * sizeof(wchar_t));

    };

    LogManager::Log(LogManager::LogLevel::Info, std::format("Successfully added \"Yggdrasil UI\" translations to : \"{}\"", skyrimTranslationsFilePath), true);

    file.close();
    return true;

};