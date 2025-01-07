#pragma once

namespace YGGDRASIL {

	struct MenuItem {

		bool confirmation;
		bool disabled;
		bool selected;

		std::string label;
		std::string text;

		std::optional<std::string> path;

		MenuItem(bool hasConfirmation, bool isDisabled, const std::string& itemLabel, std::optional<std::string> itemPath, bool isSelected, const std::string& itemText)
		: confirmation(hasConfirmation), disabled(isDisabled), label(itemLabel), path(itemPath), selected(isSelected), text(itemText) {};

	};

	enum class Global {

		Configuration,
		CurrentGamePlatform,
		CurrentLocale,
		CurrentPlatform,
		Customization,
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
		Settings,
		SKSEVersion,
		SkyrimGOG,
		SkyrimSteam,
		TESVVersion,
		YGUIVersion

	};

	enum class Manager {

		Configuration,
		Log,
		Translation,
		UI

	};

	enum class Platform {

		PC,
		PC_Gamepad,
		PlayStation,
		Xbox

	};

	class Container {

		public :

			std::vector<MenuItem> items;

			void addItem(const MenuItem& item) { items.push_back(item); };

	};

	inline std::unordered_map<Global, std::any> globalVariables;

	inline bool dataLoaded;
	inline bool initialGlobalsLoaded;
	inline bool inputLoaded;

	template <typename Type>
	Type GetGlobal(Global variable, std::string name) {

		try {

			return std::any_cast<Type>(globalVariables.at(variable));

		} catch(const std::bad_any_cast& error) {

			SKSE::stl::report_and_fail(std::format("Invalid type for global variable : {} ( {} )", name, error.what()));

		} catch(const std::out_of_range&) {

			SKSE::stl::report_and_fail(std::format("Global variable {} not found", name));

		};

	};

	bool FindGamePlatform(const char* gamePlatform);
	bool Initialize(Manager manager);
	bool IsMenuHandled(std::string menuName);

	void CloseMenu(const std::string& menuName);
	void OnSKSEMessage(SKSE::MessagingInterface::Message* message);
	void OpenMenu(const std::string& menuName);
	void ToggleDebuggingConsole(bool flag);
	void TrimLeadingSpaces(std::string& text);
	void TrimTrailingSpaces(std::string& text);

	template <typename Type>
	void SetGlobal(Global variable, Type value) { globalVariables[variable] = value; };

	std::string UTF16ToUTF8(const std::wstring& utf16String);

	std::wstring UTF8ToUTF16(const std::string& utf8String);

	RE::GFxValue MenuItemToGFxValue(const MenuItem& item, RE::GFxMovieView* view);

};