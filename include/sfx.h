#pragma once

class SFXManager {

    public :

        static SFXManager& GetSingleton() {

            static SFXManager instance;
            return instance;

        };

        void QueueSFX(const std::string& filePath);
        void CleanUp();

        ~SFXManager();

    private :

        SFXManager();

        ALCdevice* device;
        ALCcontext* context;

        std::condition_variable condition;
        std::queue<std::string> queue;
        std::mutex queueMutex;
        std::vector<std::thread> workers;

        bool terminate;

        void PlaySound(const std::string& filePath);
        void WorkerThread();

        SFXManager(const SFXManager&) = delete;
        SFXManager& operator=(const SFXManager&) = delete;

};