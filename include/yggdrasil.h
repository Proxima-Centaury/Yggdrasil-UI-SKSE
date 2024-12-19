#pragma once

namespace YGGDRASIL {

    enum class Global {

        GAMEVersion,
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
        SKSEVersion,
        SkyrimGOG,
        SkyrimSteam,
        YGUIVersion

    };

    enum class Manager {

        Configuration,
        Log,
        UI

    };

    enum class Platform {

        PC_Gamepad,
        PC_Keyboard_Mouse,
        PlayStation,
        Xbox,
        Z_Unknown

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

    void OnInputEvent(RE::InputEvent* event);
    void OnSKSEMessage(SKSE::MessagingInterface::Message* message);

    bool FindPlatform(const char* platform);
    bool Init(Manager manager);
    bool IsMenuHandled(std::string menuName);

    Platform DetectPlatform(RE::InputEvent* event);

};