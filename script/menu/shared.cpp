#include "../include/yggdrasil.h"

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* HANDLES ACTIONS THAT NEED TO BE TRIGGERED ON UI STATE END */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::UIEndState(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Executing \"UIEndState\" from UI GameDelegate call", true);

    SKSELog(args);

};


/* --------------------------------------------------------------------------------------------------------------------------------- */
/* PLAYS UI SOUND FX */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::UIPlaySound(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Executing \"UIPlaySound\" from UI GameDelegate call", true);

    SKSELog(args);

    std::string pathToUISoundFX = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PathToUISoundFX);
    std::string pathToUISoundFXFile = std::format("{}\\{}.wav", pathToUISoundFX, args[0].GetString());

    SFXManager::GetSingleton().QueueSFX(pathToUISoundFXFile);

    LogManager::Log(LogManager::LogLevel::Info, std::format("Played SFX : \"{}\"", pathToUISoundFXFile), true);

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* HANDLES ACTIONS THAT NEED TO BE TRIGGERED ON UI STATE START */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::UIStartState(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Executing \"UIStartState\" from UI GameDelegate call", true);

    SKSELog(args);

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* GETS VALUE FROM CONFIGURATION FILE */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::SKSEGetConfiguration(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Executing \"SKSEGetConfiguration\" from UI GameDelegate call", true);

    SKSELog(args);

    auto& configurationManager = ConfigurationManager::GetSingleton();
    const CSimpleIniA& configuration = configurationManager.GetConfiguration();

    auto movieClip = args.GetMovie();

    RE::GFxValue response;
    movieClip->CreateObject(&response);

    for(std::uint32_t i = 0; i < args.GetArgCount(); ++i) {

        const auto& arg = args[i];

        if(arg.GetType() != RE::GFxValue::ValueType::kString) continue;

        const char* sectionName = arg.GetString();
        const CSimpleIniA::TKeyVal* keyValues = configuration.GetSection(sectionName);

        if(keyValues != nullptr) {

            RE::GFxValue sectionObject;
            movieClip->CreateObject(&sectionObject);

            response.SetMember(sectionName, sectionObject);

            for(auto iterator = keyValues->begin(); iterator != keyValues->end(); ++iterator) {

                RE::GFxValue key, value;

                key.SetString(iterator->first.pItem);

                std::string raw = iterator->second;

                if(raw == "true" || raw == "false") {

                    value.SetBoolean(raw == "true");

                } else if(std::all_of(raw.begin(), raw.end(), ::isdigit) || (raw.find('.') != std::string::npos && raw.find_first_not_of("0123456789.") == std::string::npos)) {

                    if(raw.find('.') != std::string::npos) {

                        value.SetNumber(std::stod(raw));

                    } else {

                        value.SetNumber(std::stoi(raw));

                    };

                } else {

                    value.SetString(raw.c_str());

                };

                sectionObject.SetMember(key.GetString(), value);

            };

        };

    };

    movieClip->SetVariable("configuration", response);

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* LOGS DATA FROM UI IN YGGDRASIL UI'S LOG FILE */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::SKSELog(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Logging informations about received arguments..", false);

    if(args.GetArgCount() == 0) LogManager::Log(LogManager::LogLevel::Debug, std::format("No arguments received", args.GetArgCount()), false);
    if(args.GetArgCount() == 1) LogManager::Log(LogManager::LogLevel::Debug, std::format("Received \"{}\" argument", args.GetArgCount()), false);
    if(args.GetArgCount() > 1) LogManager::Log(LogManager::LogLevel::Debug, std::format("Received \"{}\" arguments", args.GetArgCount()), false);

    for(std::uint32_t i = 0; i < args.GetArgCount(); ++i) {

        const auto& arg = args[i];

        SKSELogProcessArgument(arg, i, 0);

    };

    LogManager::Log(LogManager::LogLevel::Debug, "Data successfully logged", true);

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* LOGS NESTED DATA FROM SKSELOG CALL */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::SKSELogProcessArgument(const RE::GFxValue& arg, std::uint32_t index, std::uint32_t depth) {

    std::string indent(depth * 2, ' ');

    if(arg.IsString()) {

        LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (String) {}", indent, index, arg.GetString()), false);

    } else if(arg.IsNumber()) {

        LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Number) {}", indent, index, arg.GetNumber()), false);

    } else if(arg.IsBool()) {

        LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Boolean) {}", indent, index, arg.GetBool()), false);

    } else if(arg.IsNull()) {

        LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Null)", indent, index), false);

    } else if(arg.IsUndefined()) {

        LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Undefined)", indent, index), false);

    } else if(arg.IsArray()) {

        LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Array) [{}]", indent, index, arg.GetArraySize()), false);

        for(std::uint32_t j = 0; j < arg.GetArraySize(); ++j) {

            RE::GFxValue arrayElement;

            if(arg.GetElement(j, &arrayElement)) {

                SKSELogProcessArgument(arrayElement, j, depth + 1);

            } else {

                LogManager::Log(LogManager::LogLevel::Debug, std::format("{}  Failed to retrieve element [{}]", indent, j), false);

            }

        };

    } else if(arg.IsObject()) {

        LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Object)", indent, index), false);

    } else {

        LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Unknown Type)", indent, index), false);

    };

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* QUITS GAME */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::SKSEQuitGame(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Executing \"SKSEQuitGame\" from UI GameDelegate call", true);

    SKSELog(args);

    SFXManager::GetSingleton().CleanUp();

    HWND hwnd = FindWindow(nullptr, L"Skyrim Special Edition");

    DWORD processID;

    if(hwnd) {

        GetWindowThreadProcessId(hwnd, &processID);

        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);

        if(hProcess) {

            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);

        };

    };

};