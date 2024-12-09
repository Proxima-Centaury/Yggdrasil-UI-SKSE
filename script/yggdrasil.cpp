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
    void Init(Manager manager) {

        SetGlobal(Global::PathToSKSEPlugins, "Data\\SKSE\\Plugins");
        SetGlobal(Global::PluginName, "Yggdrasil UI");
        SetGlobal(Global::SkyrimGOG, "Skyrim Special Edition GOG");
        SetGlobal(Global::SkyrimSteam, "Skyrim Special Edition");

        std::vector<std::string> menus = { "Main Menu" };

        SetGlobal(YGGDRASIL::Global::Menus, menus);

        if(manager == Manager::Configuration) {



        };

        if(manager == Manager::Log) {

            LogManager LogManagerInstance = LogManager::GetSingleton();

            if(LogManagerInstance.Init()) {

            };

        };

        if(manager == Manager::UI) {



        };

    };

};