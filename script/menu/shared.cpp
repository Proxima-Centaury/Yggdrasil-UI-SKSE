#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// RETURNS MENU ITEMS FOR SPECIFIC MENU
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SharedMenuManager::SKSEGetMenu(const RE::FxDelegateArgs& args) {

	LogManager::Log(LogManager::LogLevel::Debug, "Executing \"SKSEGetMenu\" from UI GameDelegate call", true);

	SKSELog(args);

	auto movieClip = args.GetMovie();

	auto section = args[1].GetString();
	auto ui = args[0].GetString();

	YGGDRASIL::Container menu;

	RE::GFxValue response;
	movieClip->CreateObject(&response);

	RE::GFxValue uiMenu;
	movieClip->CreateObject(&uiMenu);

	response.SetMember(ui, uiMenu);

	if(std::strcmp(ui, "STARTMENU") == 0) {

		if(std::strcmp(section, "MAINMENU") == 0) {

			RE::GFxValue mainmenu;
			movieClip->CreateArray(&mainmenu);

			menu.addItem(YGGDRASIL::MenuItem(true, false, "Continue", std::nullopt, false, "$YGUI_STARTMENU_MAIN_MENU_MENU_ITEM_CONTINUE"));
			menu.addItem(YGGDRASIL::MenuItem(true, false, "New", std::nullopt, true, "$YGUI_STARTMENU_MAIN_MENU_MENU_ITEM_NEW"));
			menu.addItem(YGGDRASIL::MenuItem(false, false, "Load", std::nullopt, false, "$YGUI_STARTMENU_MAIN_MENU_MENU_ITEM_LOAD"));
			menu.addItem(YGGDRASIL::MenuItem(false, false, "Settings", std::nullopt, false, "$YGUI_STARTMENU_MAIN_MENU_MENU_ITEM_SETTINGS"));
			menu.addItem(YGGDRASIL::MenuItem(false, false, "Creation Club", std::nullopt, false, "$YGUI_STARTMENU_MAIN_MENU_MENU_ITEM_CREATION_CLUB"));
			menu.addItem(YGGDRASIL::MenuItem(false, false, "Credits", std::nullopt, false, "$YGUI_STARTMENU_MAIN_MENU_MENU_ITEM_CREDITS"));
			menu.addItem(YGGDRASIL::MenuItem(true, false, "Quit", std::nullopt, false, "$YGUI_STARTMENU_MAIN_MENU_MENU_ITEM_QUIT"));

			for(size_t i = 0; i < menu.items.size(); ++i) { mainmenu.PushBack(YGGDRASIL::MenuItemToGFxValue(menu.items[i], movieClip)); };

			uiMenu.SetMember("MAINMENU", mainmenu);

		};

	};

	movieClip->SetVariable("menus", response);

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// RETURNS USER'S CURRENT CONTROLLER DEVICE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SharedMenuManager::SKSEGetPlatform(const RE::FxDelegateArgs& args) {

	LogManager::Log(LogManager::LogLevel::Debug, "Executing \"SKSEGetPlatform\" from UI GameDelegate call", true);

	SKSELog(args);

	auto movieClip = args.GetMovie();

	movieClip->SetVariable("_currentPlatform", RE::GFxValue(YGGDRASIL::GetGlobal<YGGDRASIL::Platform>(YGGDRASIL::Global::CurrentPlatform)));

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// RETURNS GAME, SKSE AND YGUI VERSIONS
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SharedMenuManager::SKSEGetVersions(const RE::FxDelegateArgs& args) {

	LogManager::Log(LogManager::LogLevel::Debug, "Executing \"SKSEGetVersions\" from UI GameDelegate call", true);

	SKSELog(args);

	auto movieClip = args.GetMovie();

	RE::GFxValue response;
	movieClip->CreateObject(&response);

	response.SetMember("GAME", RE::GFxValue(YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::GAMEVersion)));
	response.SetMember("SKSE", RE::GFxValue(YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::SKSEVersion)));
	response.SetMember("YGUI", RE::GFxValue(YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::YGUIVersion)));

	movieClip->SetVariable("versions", response);

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// LOGS DATA FROM UI IN YGGDRASIL UI'S LOG FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
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

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// LOGS NESTED DATA FROM SKSELOG CALL
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
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

			};

		};

	} else if(arg.IsObject()) {

		LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Object)", indent, index), false);

	} else {

		LogManager::Log(LogManager::LogLevel::Debug, std::format("{}Value of argument[{}] : (Unknown Type)", indent, index), false);

	};

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// QUITS GAME
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SharedMenuManager::SKSEQuitGame(const RE::FxDelegateArgs& args) {

	LogManager::Log(LogManager::LogLevel::Debug, "Executing \"SKSEQuitGame\" from UI GameDelegate call", true);

	SKSELog(args);

	SFXManager::GetSingleton().CleanUp();

	auto main = RE::Main::GetSingleton();

	if(main) main->quitGame = true;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// HANDLES ACTIONS THAT NEED TO BE TRIGGERED ON UI STATE END
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SharedMenuManager::UIEndState(const RE::FxDelegateArgs& args) {

	LogManager::Log(LogManager::LogLevel::Debug, "Executing \"UIEndState\" from UI GameDelegate call", true);

	SKSELog(args);

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// PLAYS UI SOUND FX
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SharedMenuManager::UIPlaySound(const RE::FxDelegateArgs& args) {

	LogManager::Log(LogManager::LogLevel::Debug, "Executing \"UIPlaySound\" from UI GameDelegate call", true);

	SKSELog(args);

	std::string pathToUISoundFX = YGGDRASIL::GetGlobal<const char*>(YGGDRASIL::Global::PathToUISoundFX);
	std::string pathToUISoundFXFile = std::format("{}\\{}.wav", pathToUISoundFX, args[0].GetString());

	SFXManager::GetSingleton().QueueSFX(pathToUISoundFXFile);

	LogManager::Log(LogManager::LogLevel::Info, std::format("Played SFX : \"{}\"", pathToUISoundFXFile), true);

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// HANDLES ACTIONS THAT NEED TO BE TRIGGERED ON UI STATE START
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SharedMenuManager::UIStartState(const RE::FxDelegateArgs& args) {

	LogManager::Log(LogManager::LogLevel::Debug, "Executing \"UIStartState\" from UI GameDelegate call", true);

	SKSELog(args);

};