#include "../include/yggdrasil.h"

std::unordered_map<YGGDRASIL::Global, std::any> globalVariables;

bool dataLoaded = false;
bool initialGlobalsLoaded;
bool inputLoaded = false;

namespace YGGDRASIL {

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// FINDS WHICH SKYRIM VERSION TO USE ( GOG / STEAM )
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	bool FindGamePlatform(const char* gamePlatform) {

		std::string pathToMyGames = GetGlobal<std::string>(Global::PathToMyGames);
		std::string platformFolderPath = std::format("{}\\{}\\SKSE", pathToMyGames, gamePlatform);

		if(!fs::exists(platformFolderPath)) return false;
		if(!fs::is_directory(platformFolderPath)) return false;

		SetGlobal(Global::PathToSKSE, platformFolderPath);
		return true;

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// TRIGGERS MANAGERS INITIALIZATION
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	bool Initialize(Manager manager) {

		if(!initialGlobalsLoaded) {

			SetGlobal(Global::PathToBackgrounds, "Data\\Interface\\Yggdrasil UI\\Backgrounds");
			SetGlobal(Global::PathToSKSEPlugins, "Data\\SKSE\\Plugins");
			SetGlobal(Global::PathToSkyrimInterface, "Data\\Interface");
			SetGlobal(Global::PathToUISoundFX, "Data\\Interface\\Yggdrasil UI\\SFX");
			SetGlobal(Global::PathToUITranslationsFiles, "Data\\Interface\\Yggdrasil UI\\Translations");
			SetGlobal(Global::PluginName, "Yggdrasil UI");
			SetGlobal(Global::SkyrimGOG, "Skyrim Special Edition GOG");
			SetGlobal(Global::SkyrimSteam, "Skyrim Special Edition");

			std::vector<std::string> menus = { "Main Menu" };

			SetGlobal(Global::Menus, menus);

			initialGlobalsLoaded = true;

		};

		if(manager == Manager::Configuration) {

			ConfigurationManager& ConfigurationManagerInstance = ConfigurationManager::GetSingleton();
			return ConfigurationManagerInstance.Init();

		};

		if(manager == Manager::Log) {

			LogManager& LogManagerInstance = LogManager::GetSingleton();
			return LogManagerInstance.Init();

		};

		if(manager == Manager::Translation) {

			TranslationsManager& TranslationsManagerInstance = TranslationsManager::GetSingleton();
			return TranslationsManagerInstance.Init();

		};

		return false;

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// CHECKS IF SPECIFIC MENU IS HANDLED BY YGGDRASIL UI
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	bool IsMenuHandled(std::string menuName) {

		std::vector<std::string> menus = GetGlobal<std::vector<std::string>>(Global::Menus);

		LogManager::Log(LogManager::LogLevel::Debug, std::format("Is \"{}\" handled : {}", menuName, std::find(menus.begin(), menus.end(), menuName) != menus.end()), true);
		return std::find(menus.begin(), menus.end(), menuName) != menus.end();

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// CONVERTS MENU ITEMS TO GFXVALUE
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	RE::GFxValue MenuItemToGFxValue(const MenuItem& item, RE::GFxMovieView* view) {

		RE::GFxValue object;
		view->CreateObject(&object);

		object.SetMember("confirmation", RE::GFxValue(item.confirmation));
		object.SetMember("disabled", RE::GFxValue(item.disabled));
		object.SetMember("label", RE::GFxValue(item.label.c_str()));
		object.SetMember("path", (item.path) ? RE::GFxValue(item.path->c_str()) : RE::GFxValue(RE::GFxValue::ValueType::kNull));
		object.SetMember("selected", RE::GFxValue(item.selected));
		object.SetMember("text", RE::GFxValue(item.text.c_str()));

		return object;

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// LISTENS FOR SKSE MESSAGES
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	void OnSKSEMessage(SKSE::MessagingInterface::Message* message) {

		std::string feedback;

		auto data = message->data;
		auto sender = message->sender;
		auto type = message->type;

		switch(type) {

			case SKSE::MessagingInterface::kPostLoad : {

				feedback = "All SKSE plugins are successfully loaded";
				break;

			};

			case SKSE::MessagingInterface::kPostPostLoad : {

				feedback = "??? - Message unclear ( PostPostLoad )";
				break;

			};

			case SKSE::MessagingInterface::kPreLoadGame : {

				feedback = "A save is being loaded";
				break;

			};

			case SKSE::MessagingInterface::kPostLoadGame : {

				feedback = "A save has been loaded";
				break;

			};

			case SKSE::MessagingInterface::kSaveGame : {

				feedback = "The game is being saved";
				break;

			};

			case SKSE::MessagingInterface::kDeleteGame : {

				feedback = "A save is being deleted";
				break;

			};

			case SKSE::MessagingInterface::kInputLoaded : {

				feedback = "All inputs are loaded";

				if(!YGGDRASIL::inputLoaded) {

					RE::BSInputDeviceManager::GetSingleton()->AddEventSink(UIManager::GetSingleton());

					YGGDRASIL::inputLoaded = true;

				};

				break;

			};

			case SKSE::MessagingInterface::kNewGame : {

				feedback = "A new game is starting";
				break;

			};

			case SKSE::MessagingInterface::kDataLoaded : {

				feedback = "Data is successfully loaded";

				if(!YGGDRASIL::dataLoaded) {

					RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(UIManager::GetSingleton());

					YGGDRASIL::dataLoaded = true;

				};

				break;

			};

			case SKSE::MessagingInterface::kTotal : {

				feedback = "??? - Message unclear ( Total )";
				break;

			};

		};

		LogManager::Log(LogManager::LogLevel::Info, std::format("\"{}\" sending message type {}", sender, type), false);
		LogManager::Log(LogManager::LogLevel::Info, std::format("Receiving data : {}", data), false);
		LogManager::Log(LogManager::LogLevel::Info, std::format("Message : \"{}\"", feedback), true);
		return;

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// ENABLES OR DISABLES DEBUGGING CONSOLE
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	void ToggleDebuggingConsole(bool flag) {

		if(flag) {

			AllocConsole();

			FILE* stream;

			freopen_s(&stream, "CONOUT$", "w", stdout);
			freopen_s(&stream, "CONOUT$", "w", stderr);

		};

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// TRIMS LEADING SPACES
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	void TrimLeadingSpaces(std::string& text) {

		text.erase(text.begin(), std::find_if(text.begin(), text.end(), [](unsigned char character) {

			return !std::isspace(character);

		}));

		TrimTrailingSpaces(text);

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// TRIMS TRAILING SPACES
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	void TrimTrailingSpaces(std::string& text) {

		text.erase(std::find_if(text.rbegin(), text.rend(), [](unsigned char character) {

			return !std::isspace(character);

		}).base(), text.end());

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// CONVERTS UTF8 STRINGS TO UTF16 STRINGS
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	std::wstring UTF8ToUTF16(const std::string& utf8String) {

		if(utf8String.empty()) return std::wstring();

		int sizeRequired = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);

		if(sizeRequired == 0) SKSE::stl::report_and_fail("Failed to calculate buffer size for UTF-16 conversion.");

		std::wstring utf16String(sizeRequired, L'\0');

		int conversionSizeRequired = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, utf16String.data(), sizeRequired);

		if(conversionSizeRequired == 0) SKSE::stl::report_and_fail("Failed UTF-8 to UTF-16 conversion.");

		utf16String.resize(sizeRequired - 1);
		return utf16String;

	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	// CONVERTS UTF16 STRINGS TO UTF8 STRINGS
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	std::string UTF16ToUTF8(const std::wstring& utf16String) {

		if(utf16String.empty()) return std::string();

		int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, nullptr, 0, nullptr, nullptr);

		if(sizeRequired == 0) SKSE::stl::report_and_fail("Failed to calculate buffer size for UTF-8 conversion.");

		std::string utf8String(sizeRequired, '\0');

		int conversionSizeRequired = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, utf8String.data(), sizeRequired, nullptr, nullptr);

		if(conversionSizeRequired == 0) SKSE::stl::report_and_fail("Failed UTF-16 to UTF-8 conversion.");

		utf8String.resize(sizeRequired - 1);
		return utf8String;

	};

};