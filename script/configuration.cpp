#include "../include/yggdrasil.h"

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* CREATES CONFIGURATION FILE */
/* --------------------------------------------------------------------------------------------------------------------------------- */
bool ConfigurationManager::CreateConfigurationFile(std::string pathToConfigurationFile) {

    ResetConfigurationFile();

    SI_Error configurationCreationState = configuration.SaveFile(pathToConfigurationFile.c_str());

    if(configurationCreationState < 0) return false;

    YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToConfigurationFile, pathToConfigurationFile);

    return true;

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* INITIALIZES CONFIGURATION MANAGER */
/* --------------------------------------------------------------------------------------------------------------------------------- */
bool ConfigurationManager::Init() {

    configuration.SetUnicode();

    std::string pathToSKSEPlugins = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PathToSKSEPlugins);
    std::string pluginName = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PluginName);

    std::string pathToConfigurationFile = std::format("{}\\{}.ini", pathToSKSEPlugins, pluginName);

    SI_Error configurationLoadingState = configuration.LoadFile(pathToConfigurationFile.c_str());

    if(configurationLoadingState < 0) return CreateConfigurationFile(pathToConfigurationFile);

    YGGDRASIL::SetGlobal(YGGDRASIL::Global::PathToConfigurationFile, pathToConfigurationFile);

    return true;

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* RESETS CONFIGURATION FILE */
/* --------------------------------------------------------------------------------------------------------------------------------- */
bool ConfigurationManager::ResetConfigurationFile() {

    configuration.SetValue("DEBUG", "SetLogLevel", "Trace");

    configuration.SetBoolValue("STARTMENU", "EnableCustomBackground", true);
    configuration.SetBoolValue("STARTMENU", "EnableCustomBackgroundRandomizer", true);
    configuration.SetBoolValue("STARTMENU", "EnableGameStartVideo", true);
    configuration.SetBoolValue("STARTMENU", "EnablePressStart", true);

    configuration.SetValue("STARTMENU", "SetMenuFont", "$StartMenuFont");
    configuration.SetValue("STARTMENU", "SetTextFont", "$StartMenuFont");
    configuration.SetValue("STARTMENU", "SetTitleFont", "$StartMenuFont");
    configuration.SetValue("STARTMENU", "SetMenuFontColor", "#FFFFFF");
    configuration.SetValue("STARTMENU", "SetTextFontColor", "#FFFFFF");
    configuration.SetValue("STARTMENU", "SetTitleFontColor", "#FFFFFF");

    return true;

};