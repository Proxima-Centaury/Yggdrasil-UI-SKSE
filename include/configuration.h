#pragma once

class ConfigurationManager {

    public :

        static ConfigurationManager& GetSingleton() {

            static ConfigurationManager instance;
            return instance;

        };

        bool Init();

    private :

        bool CreateConfigurationFile(std::string pathToConfigurationFile);
        bool FindConfigurationFile(std::string pathToConfigurationFile);
        bool LoadConfigurationFile(std::string pathToConfigurationFile);

};