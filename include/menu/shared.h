#pragma once

class SharedMenuManager : public RE::FxDelegateHandler {

	public :

		void Accept(CallbackProcessor* processor) override {

			LogManager::Log(LogManager::LogLevel::Debug, "Running \"SharedMenuManager::Accept\"", true);

			processor->Process("SKSEDefineEnvironment", SharedMenuManager::SKSEDefineEnvironment);
			processor->Process("SKSEGetMenu", SharedMenuManager::SKSEGetMenu);
			processor->Process("SKSEGetVersions", SharedMenuManager::SKSEGetVersions);
			processor->Process("SKSELog", SharedMenuManager::SKSELog);
			processor->Process("SKSEQuitGame", SharedMenuManager::SKSEQuitGame);

			processor->Process("UIEndState", SharedMenuManager::UIEndState);
			processor->Process("UIPlaySound", SharedMenuManager::UIPlaySound);
			processor->Process("UIStartState", SharedMenuManager::UIStartState);

		};

		static void SKSEDefineEnvironment(const RE::FxDelegateArgs& args);
		static void SKSEGetMenu(const RE::FxDelegateArgs& args);
		static void SKSEGetVersions(const RE::FxDelegateArgs& args);
		static void SKSELog(const RE::FxDelegateArgs& args);
		static void SKSELogProcessArgument(const RE::GFxValue& arg, std::uint32_t index, std::uint32_t depth);
		static void SKSEQuitGame(const RE::FxDelegateArgs& args);

		static void UIEndState(const RE::FxDelegateArgs& args);
		static void UIPlaySound(const RE::FxDelegateArgs& args);
		static void UIStartState(const RE::FxDelegateArgs& args);

};