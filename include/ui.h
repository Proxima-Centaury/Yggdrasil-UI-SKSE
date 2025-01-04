#pragma once

class UIManager : public RE::BSTEventSink<RE::MenuOpenCloseEvent>, public RE::BSTEventSink<RE::InputEvent*> {

	public :

		virtual ~UIManager() = default;
		UIManager() = default;
		UIManager(const UIManager&) = delete;
		UIManager(UIManager&&) = delete;
		UIManager& operator=(const UIManager&) = delete;
		UIManager& operator=(UIManager&&) = delete;

		static UIManager* GetSingleton() {

			static UIManager instance;
			return &instance;

		};

		RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* eventSource) override;
		RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* event, RE::BSTEventSource<RE::InputEvent*>* eventSource) override;

	private :

		RE::FxDelegate* GameDelegate { nullptr };

		bool IsXboxController();
		bool RegisterFxDelegateCallbacks(std::string menuName);
		bool UnregisterFxDelegateCallbacks(std::string menuName);

};