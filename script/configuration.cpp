#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// CREATES CONFIGURATION FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool ConfigurationManager::CreateConfigurationFile(std::string pathToConfigurationFile) {

	json configurationDefaults = {

		{ "debug", "Trace" }
		// Add audio parameters

	};

	std::ofstream configurationFile(pathToConfigurationFile);

	if(configurationFile.is_open()) {

		configurationFile << configurationDefaults.dump(4);
		configurationFile.close();

		std::cout << "Default configuration JSON file created at : " << pathToConfigurationFile << std::endl;
		return true;

	} else {

		std::cout << "Failed to create configuration JSON file at : " << pathToConfigurationFile << std::endl;
		return false;

	};

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// CHECKS IF CONFIGURATION FILE EXISTS
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool ConfigurationManager::FindConfigurationFile(std::string pathToConfigurationFile) {

	if(!fs::exists(pathToConfigurationFile)) {

		std::cout << "Configuration JSON file not found at : " << pathToConfigurationFile << std::endl;
		return false;

	} else {

		std::cout << "Configuration JSON file found at : " << pathToConfigurationFile << std::endl;
		return true;

	};

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// INITIALIZES CONFIGURATION MANAGER
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool ConfigurationManager::Init() {

	std::string pathToJSONFiles = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PathToJSONFiles, "PathToJSONFiles");
	std::string pluginName = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PluginName, "PluginName");

	std::string pathToConfigurationFile = std::format("{}\\{}.json", pathToJSONFiles, pluginName);

	if(!FindConfigurationFile(pathToConfigurationFile)) {

		if(CreateConfigurationFile(pathToConfigurationFile)) {

			return LoadConfigurationFile(pathToConfigurationFile);

		};

		return false;

	} else {

		return LoadConfigurationFile(pathToConfigurationFile);

	};

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// LOADS CONFIGURATION FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool ConfigurationManager::LoadConfigurationFile(std::string pathToConfigurationFile) {

	std::ifstream configurationFile(pathToConfigurationFile);

	if(configurationFile.is_open()) {

		json configuration;
		configurationFile >> configuration;
		configurationFile.close();

		std::cout << "Loaded configuration JSON file at : " << pathToConfigurationFile << std::endl;

		YGGDRASIL::SetGlobal(YGGDRASIL::Global::Configuration, configuration);
		return true;

	} else {

		std::cout << "Failed to load configuration JSON file at : " << pathToConfigurationFile << std::endl;
		return false;

	};

};