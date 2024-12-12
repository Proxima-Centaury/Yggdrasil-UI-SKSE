#pragma once

class UIManager : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

    public :

        static UIManager* GetSingleton() {

            static UIManager instance;

            return &instance;

        };

        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* eventSource) override;

    private :

        UIManager() = default;
        UIManager(const UIManager&) = delete;
        UIManager& operator=(const UIManager&) = delete;

        RE::FxDelegate* GameDelegate;

        bool RegisterFxDelegateCallbacks(std::string menuName);
        bool UnregisterFxDelegateCallbacks(std::string menuName);

};