#pragma once

class SharedMenuManager : public RE::FxDelegateHandler {

    public :

        void Accept(CallbackProcessor* processor) override {

            LogManager::Log(LogManager::LogLevel::Debug, "Running \"SharedMenuManager::Accept\"", true);

            // processor->Process("EndState", SharedMenuManager::EndState);
            // processor->Process("StartState", SharedMenuManager::StartState);

            processor->Process("UIPlaySound", SharedMenuManager::UIPlaySound);

            processor->Process("SKSEGetConfiguration", SharedMenuManager::SKSEGetConfiguration);
            processor->Process("SKSELog", SharedMenuManager::SKSELog);

        };


        // static void EndState(const RE::FxDelegateArgs& params);
        // static void StartState(const RE::FxDelegateArgs& params);
        static void UIPlaySound(const RE::FxDelegateArgs& args);

        static void SKSEGetConfiguration(const RE::FxDelegateArgs& params);
        static void SKSELog(const RE::FxDelegateArgs& params);
        static void SKSELogProcessArgument(const RE::GFxValue& arg, std::uint32_t index, std::uint32_t depth);

};