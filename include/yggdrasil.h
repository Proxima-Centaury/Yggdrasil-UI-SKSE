#pragma once

namespace YGGDRASIL {

	struct MenuItem {

		bool disabled;
		bool selected;

		std::string label;
		std::string text;

		std::optional<std::string> path;

		MenuItem(bool isDisabled, const std::string& itemLabel, std::optional<std::string> itemPath, bool isSelected, const std::string& itemText)
		: disabled(isDisabled), label(itemLabel), path(itemPath), selected(isSelected), text(itemText) {};

	};

	enum class Global {

		CurrentGamePlatform,
		CurrentLocale,
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
		Settings,
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

	bool FindGamePlatform(const char* gamePlatform);
	bool Initialize(Manager manager);
	bool IsMenuHandled(std::string menuName);

	void OnSKSEMessage(SKSE::MessagingInterface::Message* message);
	template <typename UnknownType>
	void SetGlobal(Global variable, UnknownType value) { globalVariables[variable] = value; };
	void ToggleDebuggingConsole(bool flag);
	void TrimLeadingSpaces(std::string& text);
	void TrimTrailingSpaces(std::string& text);

	std::string UTF16ToUTF8(const std::wstring& utf16String);

	std::wstring UTF8ToUTF16(const std::string& utf8String);

	RE::GFxValue MenuItemToGFxValue(const MenuItem& item, RE::GFxMovieView* view);

};