#include "../include/yggdrasil.h"

std::string ConfigurationManager::BuildSectionKeyString(const char* section, const char* key) const { return std::string(section) + "." + key; };

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// CREATES CONFIGURATION FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool ConfigurationManager::CreateConfigurationFile(std::string pathToConfigurationFile) {

    ResetConfigurationFile();

    SI_Error configurationCreationState = configuration.SaveFile(pathToConfigurationFile.c_str());

    if(configurationCreationState < 0) return false;

    YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToConfigurationFile, pathToConfigurationFile);
    return true;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// GETS VALUE FROM CONFIGURATION FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
template <typename ValueType>
ValueType ConfigurationManager::GetValue(const char* section, const char* key, ValueType defaultValue) const {

    std::string sectionKey = BuildSectionKeyString(section, key);

    auto iterator = cachedValues.find(sectionKey);

    if(iterator != cachedValues.end()) {

        try {

            return std::any_cast<ValueType>(iterator->second);

        } catch(const std::bad_any_cast& e) {

            LogManager::Log(LogManager::LogLevel::Error, std::format("Type mismatch for \"{}\" key : \"{}\"", sectionKey, e.what()), true);
            throw;

        };

    };

    const char* rawValue = configuration.GetValue(section, key, nullptr);

    if(!rawValue) {

        LogManager::Log(LogManager::LogLevel::Warn, std::format("\"{}\" key not found in \"{}\" section", key, section), false);
        LogManager::Log(LogManager::LogLevel::Warn, std::format("Message : \"{}\"", "Returning default value"), true);
        return defaultValue;

    };

    ValueType convertedValue;

    try {

        if constexpr(std::is_same_v<ValueType, int>) {

            convertedValue = std::stoi(rawValue);

        } else if constexpr(std::is_same_v<ValueType, bool>) {

            convertedValue = (std::string(rawValue) == "true");

        } else if constexpr(std::is_same_v<ValueType, float>) {

            convertedValue = std::stof(rawValue);

        } else {

            convertedValue = rawValue;

        };

    } catch(...) {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Failed to convert value \"{}\" for \"{}\" key", rawValue, sectionKey), false);
        LogManager::Log(LogManager::LogLevel::Error, std::format("Message : \"{}\"", "Returning default value"), true);
        return defaultValue;

    };

    cachedValues[sectionKey] = convertedValue;
    return convertedValue;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// INITIALIZES CONFIGURATION MANAGER
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool ConfigurationManager::Init() {

    configuration.SetUnicode();

    std::string pathToSKSEPlugins = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PathToSKSEPlugins);
    std::string pluginName = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PluginName);

    std::string pathToConfigurationFile = std::format("{}\\{}.ini", pathToSKSEPlugins, pluginName);

    SI_Error configurationLoadState = configuration.LoadFile(pathToConfigurationFile.c_str());

    if(configurationLoadState < 0) return CreateConfigurationFile(pathToConfigurationFile);

    YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToConfigurationFile, pathToConfigurationFile);
    return true;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// RESETS CONFIGURATION FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool ConfigurationManager::ResetConfigurationFile() {

    configuration.SetValue("DEBUG", "SetLogLevel", "Trace");

    configuration.SetValue("FONTS", "SetConsoleFont", "Default");
    configuration.SetValue("FONTS", "SetDaedricFont", "Default");
    configuration.SetValue("FONTS", "SetDialogueFont", "Default");
    configuration.SetValue("FONTS", "SetDragonFont", "Default");
    configuration.SetValue("FONTS", "SetDwemerFont", "Default");
    configuration.SetValue("FONTS", "SetEverywhereBoldFont", "Default");
    configuration.SetValue("FONTS", "SetEverywhereFont", "Default");
    configuration.SetValue("FONTS", "SetEverywhereMediumFont", "Default");
    configuration.SetValue("FONTS", "SetFalmerFont", "Default");
    configuration.SetValue("FONTS", "SetHandwrittenBoldFont", "Default");
    configuration.SetValue("FONTS", "SetHandwrittenFont", "Default");
    configuration.SetValue("FONTS", "SetMageScriptFont", "Default");
    configuration.SetValue("FONTS", "SetSkyrimBooksFont", "Default");
    configuration.SetValue("FONTS", "SetSkyrimBooksUnreadableFont", "Default");
    configuration.SetValue("FONTS", "SetSymbolsFont", "Default");
    configuration.SetValue("FONTS", "SetStartMenuFont", "Default");

    configuration.SetValue("GENERAL", "SetUIAlignment", "Left");

    configuration.SetBoolValue("STARTMENU", "EnableCustomBackground", true);
    configuration.SetBoolValue("STARTMENU", "EnableCustomBackgroundRandomizer", true);
    configuration.SetBoolValue("STARTMENU", "EnableCustomBackgroundVignette", true);

    configuration.SetValue("STARTMENU", "SetConfirmationFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetConfirmationNoFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetConfirmationYesFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetGameVersionFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetGameVersionLabelFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetMenuFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetPressStartFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetSKSEVersionFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetSKSEVersionLabelFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetTitleFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetTitleSequelFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetYggdrasilUIVersionFontColor", "0x000000");
    configuration.SetValue("STARTMENU", "SetYggdrasilUIVersionLabelFontColor", "0x000000");
    return true;

};