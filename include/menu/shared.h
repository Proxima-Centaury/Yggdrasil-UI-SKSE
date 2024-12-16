#pragma once

class SharedMenuManager : public RE::FxDelegateHandler {

    public :

        void Accept(CallbackProcessor* processor) override {

            LogManager::Log(LogManager::LogLevel::Debug, "Running \"SharedMenuManager::Accept\"", true);

            processor->Process("UIEndState", SharedMenuManager::UIEndState);
            processor->Process("UIPlaySound", SharedMenuManager::UIPlaySound);
            processor->Process("UIStartState", SharedMenuManager::UIStartState);

            processor->Process("SKSEGetConfiguration", SharedMenuManager::SKSEGetConfiguration);
            processor->Process("SKSELog", SharedMenuManager::SKSELog);

        };

        static void UIEndState(const RE::FxDelegateArgs& args);
        static void UIPlaySound(const RE::FxDelegateArgs& args);
        static void UIStartState(const RE::FxDelegateArgs& args);

        static void SKSEGetConfiguration(const RE::FxDelegateArgs& args);
        static void SKSELog(const RE::FxDelegateArgs& params);
        static void SKSELogProcessArgument(const RE::GFxValue& arg, std::uint32_t index, std::uint32_t depth);


};