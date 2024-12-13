#pragma once

class SFXManager {

    public :

        SFXManager();
        ~SFXManager();

        void AsyncPlaySound(const std::string& filePath) {

            if(!initialized) {

                LogManager::Log(LogManager::LogLevel::Error, "\"SFXManager\" not initialized", true);

                return;

            };

            std::thread([this, filePath]() {

                PlaySound(filePath);

            }).detach();

        };

    private :

        ALCdevice* device;
        ALCcontext* context;

        bool initialized = false;

        bool LoadSound(const std::string& filePath, ALuint& buffer);

        void PlaySound(const std::string& filePath);

};