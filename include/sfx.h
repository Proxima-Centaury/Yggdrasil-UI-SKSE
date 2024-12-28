#pragma once

class SFXManager {

    public :

        ~SFXManager();

        static SFXManager& GetSingleton() {

            static SFXManager instance;
            return instance;

        };

        void QueueSFX(const std::string& filePath);
        void CleanUp();

    private :

        SFXManager();
        SFXManager(const SFXManager&) = delete;
        SFXManager& operator=(const SFXManager&) = delete;

        ALCdevice* device;
        ALCcontext* context;

        std::condition_variable condition;
        std::queue<std::string> queue;
        std::mutex queueMutex;
        std::vector<std::thread> workers;

        bool terminate;

        void PlaySound(const std::string& filePath);
        void WorkerThread();

};