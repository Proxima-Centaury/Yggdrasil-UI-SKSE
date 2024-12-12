#pragma once

class ConfigurationManager {

    public :

        static ConfigurationManager& GetSingleton() {

            static ConfigurationManager instance;

            return instance;

        };

        bool Init();

        template <typename ValueType>
        ValueType GetValue(const char* section, const char* key, ValueType defaultValue = ValueType()) const;

        CSimpleIniA& GetConfiguration() { return configuration; };

    private :

        CSimpleIniA configuration;

        mutable std::unordered_map<std::string, std::any> cachedValues;

        bool CreateConfigurationFile(std::string pathToConfigurationFile);
        bool ResetConfigurationFile();

        std::string BuildSectionKeyString(const char* section, const char* key) const;

};