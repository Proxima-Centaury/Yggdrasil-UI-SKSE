#include "../include/yggdrasil.h"

std::unordered_map<YGGDRASIL::Global, std::any> globalVariables;

namespace YGGDRASIL {

    /* --------------------------------------------------------------------------------------------------------------------------------- */
    /* FINDS WHICH SKYRIM VERSION TO USE ( GOG / STEAM ) */
    /* --------------------------------------------------------------------------------------------------------------------------------- */
    bool FindPlatform(const char* platform) {

        std::string pathToMyGames = GetGlobal<std::string>(YGGDRASIL::Global::PathToMyGames);
        std::string platformFolderPath = std::format("{}\\{}\\SKSE", pathToMyGames, platform);

        if(!fs::exists(platformFolderPath)) return false;
        if(!fs::is_directory(platformFolderPath)) return false;

        SetGlobal(Global::PathToSKSE, platformFolderPath);

        return true;

    };

    /* --------------------------------------------------------------------------------------------------------------------------------- */
    /* TRIGGERS MANAGERS INITIALIZATION */
    /* --------------------------------------------------------------------------------------------------------------------------------- */
    bool Init(Manager manager) {

        SetGlobal(Global::PathToSKSEPlugins, "Data\\SKSE\\Plugins");
        SetGlobal(Global::PathToUISoundFX, "Data\\Interface\\Yggdrasil UI\\SFX");
        SetGlobal(Global::PluginName, "Yggdrasil UI");
        SetGlobal(Global::SkyrimGOG, "Skyrim Special Edition GOG");
        SetGlobal(Global::SkyrimSteam, "Skyrim Special Edition");

        std::vector<std::string> menus = { "Main Menu" };

        SetGlobal(YGGDRASIL::Global::Menus, menus);

        if(manager == Manager::Configuration) {

            ConfigurationManager& ConfigurationManagerInstance = ConfigurationManager::GetSingleton();

            return ConfigurationManagerInstance.Init();

        };

        if(manager == Manager::Log) {

            LogManager& LogManagerInstance = LogManager::GetSingleton();

            return LogManagerInstance.Init();

        };

        return false;

    };

    /* --------------------------------------------------------------------------------------------------------------------------------- */
    /* CHECKS IF SPECIFIC MENU IS HANDLED BY YGGDRASIL UI */
    /* --------------------------------------------------------------------------------------------------------------------------------- */
    bool YGGDRASIL::IsMenuHandled(std::string menuName) {

        std::vector<std::string> menus = YGGDRASIL::GetGlobal<std::vector<std::string>>(YGGDRASIL::Global::Menus);

        LogManager::Log(LogManager::LogLevel::Debug, std::format("Is \"{}\" handled : {}", menuName, std::find(menus.begin(), menus.end(), menuName) != menus.end()), true);

        return std::find(menus.begin(), menus.end(), menuName) != menus.end();

    };

    /* --------------------------------------------------------------------------------------------------------------------------------- */
    /* LISTENS FOR SKSE MESSAGES */
    /* --------------------------------------------------------------------------------------------------------------------------------- */
    void YGGDRASIL::OnSKSEMessage(SKSE::MessagingInterface::Message* message) {

        std::string feedback;

        auto data = message->data;
        auto sender = message->sender;
        auto type = message->type;

        switch(type) {

            case SKSE::MessagingInterface::kPostLoad : {

                feedback = "All SKSE plugins are successfully loaded";
                break;

            };

            case SKSE::MessagingInterface::kPostPostLoad : {

                feedback = "??? - Message unclear ( PostPostLoad )";
                break;

            };

            case SKSE::MessagingInterface::kPreLoadGame : {

                feedback = "A save is being loaded";
                break;

            };

            case SKSE::MessagingInterface::kPostLoadGame : {

                feedback = "A save has been loaded";
                break;

            };

            case SKSE::MessagingInterface::kSaveGame : {

                feedback = "The game is being saved";
                break;

            };

            case SKSE::MessagingInterface::kDeleteGame : {

                feedback = "A save is being deleted";
                break;

            };

            case SKSE::MessagingInterface::kInputLoaded : {

                feedback = "All inputs are loaded";
                break;

            };

            case SKSE::MessagingInterface::kNewGame : {

                feedback = "A new game is starting";
                break;

            };

            case SKSE::MessagingInterface::kDataLoaded : {

                feedback = "Data is successfully loaded";
                auto UI = UIManager::GetSingleton();
                RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(UI);
                break;

            };

            case SKSE::MessagingInterface::kTotal : {

                feedback = "??? - Message unclear ( Total )";
                break;

            };
        };

        LogManager::Log(LogManager::LogLevel::Info, std::format("\"{}\" sending message type {}", sender, type), false);
        LogManager::Log(LogManager::LogLevel::Info, std::format("Receiving data : {}", data), false);
        LogManager::Log(LogManager::LogLevel::Info, std::format("Message : \"{}\"", feedback), true);

        return;

    };

};