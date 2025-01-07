#pragma once

class TranslationsManager {

    public :

        enum class Languages {

            Chinese,
            English,
            French,
            German,
            Italian,
            Japanese,
            Polish,
            Russian,
            Spanish

        };

        const std::unordered_map<std::string, Languages> languageMap = {

            { "CHINESE", Languages::Chinese },
            { "ENGLISH", Languages::English },
            { "FRENCH", Languages::French },
            { "GERMAN", Languages::German },
            { "ITALIAN", Languages::Italian },
            { "JAPANESE", Languages::Japanese },
            { "POLISH", Languages::Polish },
            { "RUSSIAN", Languages::Russian },
            { "SPANISH", Languages::Spanish }

        };

        static TranslationsManager& GetSingleton() {

            static TranslationsManager instance;
            return instance;

        };

        bool Init();

    private :

        std::unordered_map<std::string, std::string> translations;

        std::unordered_set<std::string> existingKeys;

        Languages GetCurrentGameLanguage();

};