#pragma once

class ConfigurationManager {

    public :

        static ConfigurationManager& GetSingleton() {

            static ConfigurationManager instance;

            return instance;

        };

        bool Init();

    private :

        CSimpleIniA configuration;

        bool CreateConfigurationFile(std::string pathToConfigurationFile);
        bool ResetConfigurationFile();

};