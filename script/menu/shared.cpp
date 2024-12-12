#include "../include/yggdrasil.h"

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* PLAYS UI SOUND FX */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::UIPlaySound(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Executing \"UIPlaySound\" from UI GameDelegate call", true);

    SKSELog(args);

    std::string pathToUISoundFX = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PathToUISoundFX);

    // auto* soundSystem = RE::BSAudioManager::GetSingleton();

    // if(!soundSystem) {

    //     LogManager::Log(LogManager::LogLevel::Error, "\"BSAudioManager\" not found", false);
    //     LogManager::Log(LogManager::LogLevel::Error, "Message : \"UI won't be able to trigger UIPlaySound\" SFX", true);

    //     return;

    // }

    // std::string soundPath = std::format("{}\\{}", pathToUISoundFX, args[0].GetString());
    // RE::BGSSoundDescriptorForm* soundDescriptor = RE::TESForm::LookupByEditorID<RE::BGSSoundDescriptorForm>(soundPath);

    // soundSystem->Play(soundDescriptor);

}

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* GETS VALUE FROM CONFIGURATION FILE */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::SKSEGetConfiguration(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Executing \"SKSEGetConfiguration\" from UI GameDelegate call", true);

    SKSELog(args);

    auto& configurationManager = ConfigurationManager::GetSingleton();

    auto movieClip = args.GetMovie();

    RE::GFxValue returnValue;
    movieClip->CreateObject(&returnValue);

    const CSimpleIniA& configuration = configurationManager.GetConfiguration();

    CSimpleIniA::TNamesDepend sections;
    configuration.GetAllSections(sections);

    for(const auto& section : sections) {

        RE::GFxValue sectionObject;
        movieClip->CreateObject(&sectionObject);

        CSimpleIniA::TNamesDepend keys;
        configuration.GetAllKeys(section.pItem, keys);

        for(const auto& key : keys) {

            const char* value = configuration.GetValue(section.pItem, key.pItem, nullptr);

            RE::GFxValue keyValue;
            movieClip->CreateString(&keyValue, (value) ? value : "");

            sectionObject.SetMember(key.pItem, keyValue);

        };

        returnValue.SetMember(section.pItem, sectionObject);

    };

    RE::GFxValue object = args[0];
    if(object.IsObject()) object.SetMember("configuration", returnValue);

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* LOGS DATA FROM UI IN YGGDRASIL UI'S LOG FILE */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SharedMenuManager::SKSELog(const RE::FxDelegateArgs& args) {

    LogManager::Log(LogManager::LogLevel::Debug, "Logging informations about received arguments...", false);

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