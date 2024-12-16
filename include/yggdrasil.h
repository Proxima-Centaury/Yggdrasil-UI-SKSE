#pragma once

namespace YGGDRASIL {

    enum class Global {

        Menus,
        MenuState,
        PathToBackgrounds,
        PathToConfigurationFile,
        PathToLogFile,
        PathToMyGames,
        PathToMyDocuments,
        PathToSKSE,
        PathToSKSEPlugins,
        PathToUISoundFX,
        PluginName,
        SkyrimGOG,
        SkyrimSteam

    };

    enum class Manager {

        Configuration,
        Log,
        UI

    };

    inline std::unordered_map<Global, std::any> globalVariables;

    template <typename UnknownType>
    UnknownType GetGlobal(Global variable) {

        try {

            return std::any_cast<UnknownType>(globalVariables.at(variable));

        } catch(const std::bad_any_cast& error) {

            SKSE::stl::report_and_fail(std::string("Invalid type for global variable : ") + error.what());

        } catch(const std::out_of_range&) {

            SKSE::stl::report_and_fail("Global variable not found.");

        };

    };

    template <typename UnknownType>
    void SetGlobal(Global variable, UnknownType value) {

        globalVariables[variable] = value;

    };

    void OnSKSEMessage(SKSE::MessagingInterface::Message* message);

    bool FindPlatform(const char* platform);
    bool Init(Manager manager);
    bool IsMenuHandled(std::string menuName);

};