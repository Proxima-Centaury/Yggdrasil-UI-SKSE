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

    configuration.SetBoolValue("STARTMENU", "EnableCustomBackground", true);
    configuration.SetBoolValue("STARTMENU", "EnableCustomBackgroundRandomizer", true);
    configuration.SetBoolValue("STARTMENU", "EnableCustomBackgroundVignette", true);

    configuration.SetValue("STARTMENU", "SetMainMenuPosition", "Left");
    configuration.SetValue("STARTMENU", "SetMenuFont", "$StartMenuFont");
    configuration.SetValue("STARTMENU", "SetTextFont", "$StartMenuFont");
    configuration.SetValue("STARTMENU", "SetTitleFont", "$StartMenuFont");
    configuration.SetValue("STARTMENU", "SetMenuFontColor", "#FFFFFF");
    configuration.SetValue("STARTMENU", "SetTextFontColor", "#FFFFFF");
    configuration.SetValue("STARTMENU", "SetTitleFontColor", "#FFFFFF");
    return true;

};