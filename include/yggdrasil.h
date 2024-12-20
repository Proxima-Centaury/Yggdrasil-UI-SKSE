#pragma once

namespace YGGDRASIL {

    enum class Global {

        CurrentPlatform,
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
        PathToSkyrimInterface,
        PathToUISoundFX,
        PathToUITranslationsFiles,
        PluginName,
        SKSEVersion,
        SkyrimGOG,
        SkyrimSteam,
        YGUIVersion

    };

    enum class Manager {

        Configuration,
        Log,
        Translation,
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

    bool FindPlatform(const char* platform);
    bool Init(Manager manager);
    bool IsMenuHandled(std::string menuName);

    void OnSKSEMessage(SKSE::MessagingInterface::Message* message);
    template <typename UnknownType>
    void SetGlobal(Global variable, UnknownType value) { globalVariables[variable] = value; };
    void ToggleDebuggingConsole(bool flag);
    void TrimLeadingSpaces(std::string& text);
    void TrimTrailingSpaces(std::string& text);

    std::string UTF16ToUTF8(const std::wstring& utf16String);

    std::wstring UTF8ToUTF16(const std::string& utf8String);

};