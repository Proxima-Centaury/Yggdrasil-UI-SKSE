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

						std::cout << std::format("\"Keyboard\" input detected : {}", key) << "\n";
						YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::Platform::PC);
						break;

					};

					case RE::INPUT_DEVICE::kMouse : {

						std::cout << std::format("\"Mouse\" input detected : {}", key) << "\n";
						YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::Platform::PC);
						break;

					};

					case RE::INPUT_DEVICE::kGamepad : {

						std::cout << std::format("\"Gamepad\" input detected : {}", key) << "\n";

						if(IsXboxController()) {

							std::cout << std::format("\"Gamepad\" type is : \"{}\"", "Xbox controller") << "\n";
							YGGDRASIL::SetGlobal(YGGDRASIL::Global::CurrentPlatform, YGGDRASIL::Platform::Xbox);

						} else {

							std::cout << std::format("\"Gamepad\" type is : \"{}\"", "PlayStation controller") << "\n";
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

			movieClip->SetVariable("SKSEGameLocale", YGGDRASIL::GetGlobal<std::string>(YGGDRASIL::Global::CurrentLocale, "CurrentLocale").c_str());
			LogManager::Log(LogManager::LogLevel::Information, std::format("Setting SKSEGameLocale variable in \"{}\"", movieClipName.GetString()), false);
			RE::GFxValue SKSEGameLocale;
			movieClip->GetVariable(&SKSEGameLocale, "SKSEGameLocale");
			LogManager::Log(LogManager::LogLevel::Information, std::format("Checking SKSEGameLocale value : \"{}\"", SKSEGameLocale.GetString()), true);

			movieClip->SetVariable("SKSEHasLoaded", true);
			LogManager::Log(LogManager::LogLevel::Information, std::format("Setting SKSEHasLoaded variable in \"{}\"", movieClipName.GetString()), false);
			RE::GFxValue SKSEHasLoaded;
			movieClip->GetVariable(&SKSEHasLoaded, "SKSEHasLoaded");
			LogManager::Log(LogManager::LogLevel::Information, std::format("Checking SKSEHasLoaded value : {}", SKSEHasLoaded.GetBool()), true);

			movieClip->SetVariable("UIDelegateHandler", "External");
			LogManager::Log(LogManager::LogLevel::Information, std::format("Setting UIDelegateHandler variable in \"{}\"", movieClipName.GetString()), false);
			RE::GFxValue UIDelegateHandler;
			movieClip->GetVariable(&UIDelegateHandler, "UIDelegateHandler");
			LogManager::Log(LogManager::LogLevel::Information, std::format("Checking UIDelegateHandler value : \"{}\"", UIDelegateHandler.GetString()), true);

			LogManager::Log(LogManager::LogLevel::Information, std::format("Is \"GameDelegate\" callback list for \"{}\" empty : {}", menuName, !areCallbacksRegistered), false);
			LogManager::Log(LogManager::LogLevel::Information, std::format("Callbacks for \"{}\" successfully registered", menuName), true);
			return true;

		} else {

			movieClip->SetVariable("UIDelegateHandler", "Internal");
			LogManager::Log(LogManager::LogLevel::Information, std::format("Setting UIDelegateHandler variable in \"{}\"", movieClipName.GetString()), false);
			RE::GFxValue UIDelegateHandler;
			movieClip->GetVariable(&UIDelegateHandler, "UIDelegateHandler");
			LogManager::Log(LogManager::LogLevel::Information, std::format("Checking UIDelegateHandler value : \"{}\"", UIDelegateHandler.GetString()), true);

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