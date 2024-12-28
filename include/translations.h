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

        const std::unordered_map<Languages, std::string> languageFiles = {

            { Languages::Chinese, "translate_chinese.txt" },
            { Languages::English, "translate_english.txt" },
            { Languages::French, "translate_french.txt" },
            { Languages::German, "translate_german.txt" },
            { Languages::Italian, "translate_italian.txt" },
            { Languages::Japanese, "translate_japanese.txt" },
            { Languages::Polish, "translate_polish.txt" },
            { Languages::Russian, "translate_russian.txt" },
            { Languages::Spanish, "translate_spanish.txt" }

        };

        static TranslationsManager& GetSingleton() {

            static TranslationsManager instance;
            return instance;

        };

        bool Init();

    private :

        std::unordered_map<std::string, std::string> translations;

        std::unordered_set<std::string> existingKeys;

        bool ReadExistingTranslationKeys(const std::string& filePath);
        bool ReadTranslationsFile(const std::string& filePath);
        bool WriteInTranslationsFile(const std::string& skyrimTranslationsFileName);

        Languages GetCurrentGameLanguage();

        std::string GetSkyrimTranslationsFileName();

};