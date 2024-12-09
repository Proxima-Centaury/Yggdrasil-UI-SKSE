#pragma once

class ConfigurationManager {

    public :

        static ConfigurationManager& GetSingleton() {

            static ConfigurationManager instance;

            return instance;

        };

        bool Init();

        CSimpleIniA& GetConfiguration() { return configuration; };

    private :

        CSimpleIniA configuration;

        bool CreateConfigurationFile(std::string pathToConfigurationFile);
        bool ResetConfigurationFile();

};