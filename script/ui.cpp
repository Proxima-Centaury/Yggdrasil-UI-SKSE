#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// LISTENS FOR MENU OPEN AND CLOSE EVENTS
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
RE::BSEventNotifyControl UIManager::ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {

	if(!event) return RE::BSEventNotifyControl::kContinue;

	std::string menuName = event->menuName.c_str();

	LogManager::Log(LogManager::LogLevel::Information, std::format("\"{}\" is opening : {}", menuName, event->opening), false);
	LogManager::Log(LogManager::LogLevel::Information, std::format("\"{}\" is closing : {}", menuName, !event->opening), true);

	if(event->opening) {

		if(YGGDRASIL::IsMenuHandled(menuName)) RegisterFxDelegateCallbacks(menuName);

	} else {

		if(YGGDRASIL::IsMenuHandled(menuName)) UnregisterFxDelegateCallbacks(menuName);

	};

	return RE::BSEventNotifyControl::kContinue;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// LISTENS FOR INPUT EVENTS
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
RE::BSEventNotifyControl UIManager::ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>*) {

	if(!*event) return RE::BSEventNotifyControl::kContinue;

	for(RE::InputEvent *inputEvent = *event; inputEvent; inputEvent = inputEvent->next) {

		if(inputEvent->eventType.get() == RE::INPUT_EVENT_TYPE::kButton) {

			auto buttonEvent = inputEvent->AsButtonEvent();

			if(buttonEvent && buttonEvent->IsDown()) {

				auto device = buttonEvent->GetDevice();
				auto key = buttonEvent->GetIDCode();

				switch(device) {

					case RE::INPUT_DEVICE::kKeyboard : {

						std::cout << std::format("\"Keyboard\" input detected : {}", key) << std::endl;
						YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::Platform::PC);
						break;

					};

					case RE::INPUT_DEVICE::kMouse : {

						std::cout << std::format("\"Mouse\" input detected : {}", key) << std::endl;
						YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::Platform::PC);
						break;

					};

					case RE::INPUT_DEVICE::kGamepad : {

						std::cout << std::format("\"Gamepad\" input detected : {}", key) << std::endl;

						if(IsXboxController()) {

							std::cout << std::format("\"Gamepad\" type is : \"{}\"", "Xbox controller") << std::endl;
							YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::Platform::Xbox);

						} else {

							std::cout << std::format("\"Gamepad\" type is : \"{}\"", "PlayStation controller") << std::endl;
							YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::Platform::PlayStation);

						};

						break;

					};

				};

			};

		};

	};

	return RE::BSEventNotifyControl::kContinue;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// CREATES AN ACTION SCRIPT OBJECT IN CURRENT MENU'S ROOT LEVEL WITH VARIOUS PROPERTIES AND METHODS
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool UIManager::CreateUIActionScriptObject(RE::GFxMovieView *movieClip, std::string menuName) {

	std::string language = std::format("{}", YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::CurrentLanguage, "CurrentLanguage"));
	std::transform(language.begin(), language.end(), language.begin(), [](unsigned char c) { return std::tolower(c); });

	auto basePath = YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::BasePath, "BasePath");
	auto pathToStyles = YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::PathToStyles, "PathToStyles");
	auto pathToTranslations = YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::PathToTranslations, "PathToTranslations");

	RE::GFxValue SKSE;
	movieClip->CreateObject(&SKSE, {});

	RE::GFxValue customization;
	movieClip->CreateObject(&customization, {});

	if(menuName == "Main Menu") {

		auto fullPathToStyles = std::format("{}{}\\{}", basePath, pathToStyles, "startmenu.json");
		std::ifstream startmenuCustomizationFile(fullPathToStyles);

		json style;
		startmenuCustomizationFile >> style;
		startmenuCustomizationFile.close();

		RE::GFxValue startmenu;
		movieClip->CreateObject(&startmenu, {});

		customization.SetMember("startmenu", style.dump().c_str());
		LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"startmenu\" property to \"{}\"", "_root.SKSE.customization"), false);

		SKSE.SetMember("customization", customization);
		LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"cutomization\" property to \"{}\"", "_root.SKSE"), false);

	};

	RE::GFxValue data;
	movieClip->CreateObject(&data, {});

	data.SetMember("environment", "Skyrim");
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"environment\" property to \"{}\"", "_root.SKSE.data"), false);

	RE::GFxValue paths;
	movieClip->CreateObject(&paths, {});

	paths.SetMember("backgrounds", YGGDRASIL::FormatPathForSWF(YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::PathToBackgrounds, "PathToBackgrounds")).c_str());
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"backgrounds\" property to \"{}\"", "_root.SKSE.data.paths"), false);

	paths.SetMember("icons", YGGDRASIL::FormatPathForSWF(YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::PathToIcons, "PathToIcons")).c_str());
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"icons\" property to \"{}\"", "_root.SKSE.data.paths"), false);

	data.SetMember("paths", paths);
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"paths\" property to \"{}\"", "_root.SKSE.data"), false);

	RE::GFxValue versions;
	movieClip->CreateObject(&versions, {});

	versions.SetMember("SKSE", YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::SKSEVersion, "SKSEVersion").c_str());
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"SKSE\" property to \"{}\"", "_root.SKSE.data.versions"), false);

	versions.SetMember("TESV", YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::TESVVersion, "TESVVersion").c_str());
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"TESV\" property to \"{}\"", "_root.SKSE.data.versions"), false);

	versions.SetMember("YGUI", YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::YGUIVersion, "YGUIVersion").c_str());
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"YGUI\" property to \"{}\"", "_root.SKSE.data.versions"), false);

	data.SetMember("versions", versions);
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"versions\" property to \"{}\"", "_root.SKSE.data"), false);

	SKSE.SetMember("data", data);
	LogManager::Log(LogManager::LogLevel::Information, std::format("Adding \"data\" property to \"{}\"", "_root.SKSE"), true);

	RE::GFxValue translation;

	auto fullPathToTranslations = std::format("{}{}\\{}.json", basePath, pathToTranslations, language);
	std::ifstream translationFile(fullPathToTranslations);

	json translations;
	translationFile >> translations;
	translationFile.close();

	movieClip->CreateString(&translation, translations.dump().c_str());

	SKSE.SetMember("translation", translation);

	movieClip->SetVariable("SKSE", SKSE);

	movieClip->Invoke("_root._initialize", nullptr, nullptr, 0);
	return true;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// CHECKS WHETHER CONTROLLER IS A XBOX ONE OR NOT
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool UIManager::IsXboxController() {

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	return XInputGetState(0, &state) == ERROR_SUCCESS;

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// REGISTERS CALLBACKS FOR CURRENT MENU
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool UIManager::RegisterFxDelegateCallbacks(std::string menuName) {

	auto menu = RE::UI::GetSingleton()->GetMenu(menuName);
	auto movieClip = menu->uiMovie.get();

	GameDelegate = new RE::FxDelegate();

	SharedMenuManager sharedMenuManager;

	if(movieClip) {

		RE::GFxValue movieClipName;
		movieClip->GetVariable(&movieClipName, "_name");
		LogManager::Log(LogManager::LogLevel::Information, std::format("\"{}\" MovieClip found in \"{}\"", movieClipName.GetString(), menuName), true);

		movieClip->SetState(RE::GFxState::StateType::kExternalInterface, GameDelegate);

		GameDelegate->RegisterHandler(&sharedMenuManager);

		bool areCallbacksRegistered = !GameDelegate->callbacks.IsEmpty();

		if(areCallbacksRegistered) {

			CreateUIActionScriptObject(movieClip, menuName);

			if(menuName == "Main Menu")

			LogManager::Log(LogManager::LogLevel::Information, std::format("Is \"GameDelegate\" callback list for \"{}\" empty : {}", menuName, !areCallbacksRegistered), false);
			LogManager::Log(LogManager::LogLevel::Information, std::format("Callbacks for \"{}\" successfully registered", menuName), true);
			return true;

		} else {

			LogManager::Log(LogManager::LogLevel::Warning, std::format("Failed to register callbacks for \"{}\"", menuName), false);
			LogManager::Log(LogManager::LogLevel::Warning, std::format("May be due to : \"Possibly no callbacks to register\"", menuName), false);
			LogManager::Log(LogManager::LogLevel::Warning, std::format("May be due to : \"Menu callbacks registration possibly faced unhandled issue\"", menuName), true);
			return false;

		};

	} else {

		LogManager::Log(LogManager::LogLevel::Warning, std::format("No MovieClip found in \"{}\"", menuName), true);
		return false;

	};

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// UNREGISTERS CALLBACKS FOR CURRENT MENU
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
bool UIManager::UnregisterFxDelegateCallbacks(std::string menuName) {

	SharedMenuManager sharedMenuManager;

	GameDelegate->UnregisterHandler(&sharedMenuManager);

	bool areCallbacksUnregistered = GameDelegate->callbacks.IsEmpty();

	if(areCallbacksUnregistered) {

		LogManager::Log(LogManager::LogLevel::Information, std::format("{} callbacks unregistered", menuName), true);
		return true;

	} else {

		LogManager::Log(LogManager::LogLevel::Warning, std::format("Failed to unregister callbacks for {}", menuName), false);
		LogManager::Log(LogManager::LogLevel::Warning, std::format("May be due to : \"Possibly no callbacks to unregister\"", menuName), false);
		LogManager::Log(LogManager::LogLevel::Warning, std::format("May be due to : \"Menu callbacks registration possibly faced unhandled issue\"", menuName), true);
		return false;

	};

};