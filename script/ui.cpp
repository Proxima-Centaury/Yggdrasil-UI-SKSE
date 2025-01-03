#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// LISTENS FOR MENU OPEN AND CLOSE EVENTS
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
RE::BSEventNotifyControl UIManager::ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {

	std::string menuName = event->menuName.c_str();

	LogManager::Log(LogManager::LogLevel::Info, std::format("\"{}\" is opening : {}", menuName, event->opening), false);
	LogManager::Log(LogManager::LogLevel::Info, std::format("\"{}\" is closing : {}", menuName, !event->opening), true);

	if(event->opening) {

		if(YGGDRASIL::IsMenuHandled(menuName)) RegisterFxDelegateCallbacks(menuName);

	} else {

		if(YGGDRASIL::IsMenuHandled(menuName)) UnregisterFxDelegateCallbacks(menuName);

	};

	return RE::BSEventNotifyControl::kContinue;

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

		LogManager::Log(LogManager::LogLevel::Info, std::format("\"{}\" MovieClip found in \"{}\"", movieClipName.GetString(), menuName), true);

		movieClip->SetState(RE::GFxState::StateType::kExternalInterface, GameDelegate);

		GameDelegate->RegisterHandler(&sharedMenuManager);
		
		bool areCallbacksRegistered = !GameDelegate->callbacks.IsEmpty();

		if(areCallbacksRegistered) {

			movieClip->SetVariable("SKSEGameLocale", YGGDRASIL::GetGlobal<const char *>(YGGDRASIL::Global::CurrentLocale));

			LogManager::Log(LogManager::LogLevel::Info, std::format("Setting SKSEGameLocale variable in \"{}\"", movieClipName.GetString()), false);

			RE::GFxValue SKSEGameLocale;
			movieClip->GetVariable(&SKSEGameLocale, "SKSEGameLocale");

			LogManager::Log(LogManager::LogLevel::Info, std::format("Checking SKSEGameLocale value : \"{}\"", SKSEGameLocale.GetString()), true);

			movieClip->SetVariable("SKSEHasLoaded", true);

			LogManager::Log(LogManager::LogLevel::Info, std::format("Setting SKSEHasLoaded variable in \"{}\"", movieClipName.GetString()), false);

			RE::GFxValue SKSEHasLoaded;
			movieClip->GetVariable(&SKSEHasLoaded, "SKSEHasLoaded");

			LogManager::Log(LogManager::LogLevel::Info, std::format("Checking SKSEHasLoaded value : {}", SKSEHasLoaded.GetBool()), true);

			movieClip->SetVariable("UIDelegateHandler", "External");

			LogManager::Log(LogManager::LogLevel::Info, std::format("Setting UIDelegateHandler variable in \"{}\"", movieClipName.GetString()), false);

			RE::GFxValue UIDelegateHandler;
			movieClip->GetVariable(&UIDelegateHandler, "UIDelegateHandler");

			LogManager::Log(LogManager::LogLevel::Info, std::format("Checking UIDelegateHandler value : \"{}\"", UIDelegateHandler.GetString()), true);

			LogManager::Log(LogManager::LogLevel::Info, std::format("Is \"GameDelegate\" callback list for \"{}\" empty : {}", menuName, !areCallbacksRegistered), false);
			LogManager::Log(LogManager::LogLevel::Info, std::format("Callbacks for \"{}\" successfully registered", menuName), true);
			return true;

		} else {

			movieClip->SetVariable("UIDelegateHandler", "Internal");

			LogManager::Log(LogManager::LogLevel::Info, std::format("Setting UIDelegateHandler variable in \"{}\"", movieClipName.GetString()), false);

			RE::GFxValue UIDelegateHandler;
			movieClip->GetVariable(&UIDelegateHandler, "UIDelegateHandler");

			LogManager::Log(LogManager::LogLevel::Info, std::format("Checking UIDelegateHandler value : \"{}\"", UIDelegateHandler.GetString()), true);

			LogManager::Log(LogManager::LogLevel::Warn, std::format("Failed to register callbacks for \"{}\"", menuName), false);
			LogManager::Log(LogManager::LogLevel::Warn, std::format("May be due to : \"Possibly no callbacks to register\"", menuName), false);
			LogManager::Log(LogManager::LogLevel::Warn, std::format("May be due to : \"Menu callbacks registration possibly faced unhandled issue\"", menuName), true);
			return false;

		};

	} else {

		LogManager::Log(LogManager::LogLevel::Warn, std::format("No MovieClip found in \"{}\"", menuName), true);
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

		LogManager::Log(LogManager::LogLevel::Info, std::format("{} callbacks unregistered", menuName), true);
		return true;

	} else {

		LogManager::Log(LogManager::LogLevel::Warn, std::format("Failed to unregister callbacks for {}", menuName), false);
		LogManager::Log(LogManager::LogLevel::Warn, std::format("May be due to : \"Possibly no callbacks to unregister\"", menuName), false);
		LogManager::Log(LogManager::LogLevel::Warn, std::format("May be due to : \"Menu callbacks registration possibly faced unhandled issue\"", menuName), true);
		return false;

	};

};