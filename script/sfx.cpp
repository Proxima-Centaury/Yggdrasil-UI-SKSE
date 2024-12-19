#include "../include/yggdrasil.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// INITIALIZES OPENAL FOR SFX
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
SFXManager::SFXManager() : device(nullptr), context(nullptr), terminate(false) {

    device = alcOpenDevice(nullptr);

    if(!device) {

        LogManager::Log(LogManager::LogLevel::Error, "Failed to open \"OpenAL\" device", true);
        return;

    };

    context = alcCreateContext(device, nullptr);

    if(!context || !alcMakeContextCurrent(context)) {

        LogManager::Log(LogManager::LogLevel::Error, "Failed to set \"OpenAL\" context", true);
        if (context) alcDestroyContext(context);
        alcCloseDevice(device);
        return;

    };

    for(int i = 0; i < std::thread::hardware_concurrency(); ++i) { workers.emplace_back([this]() { WorkerThread(); }); };

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// CLEARS OPENAL RELATED DATA ON SFXMANAGER DESTRUCTION
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
SFXManager::~SFXManager() { CleanUp(); };

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// CLEANS UP OPENAL DATA
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SFXManager::CleanUp() {

    LogManager::Log(LogManager::LogLevel::Debug, "Executing \"SFXManager\" cleanup", true);

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        terminate = true;
    }

    condition.notify_all();

    for(std::thread& thread : workers) { if(thread.joinable()) thread.join(); };

    if(context) {

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);

    };

    if(device) { alcCloseDevice(device); };

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// PLAYS SFX FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SFXManager::PlaySound(const std::string& filePath) {

    std::ifstream file(filePath, std::ios::binary);

    if(!file) {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Failed to open \"SFX\" file : \"{}\"", filePath), true);
        return;

    };

    char chunkId[4];

    file.read(chunkId, 4);

    if(std::strncmp(chunkId, "RIFF", 4) != 0) {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Invalid \"SFX\" file format : \"{}\"", filePath), true);
        return;

    };

    file.seekg(22);
    short channels;
    file.read(reinterpret_cast<char*>(&channels), sizeof(short));

    file.seekg(24);
    int sampleRate;
    file.read(reinterpret_cast<char*>(&sampleRate), sizeof(int));

    file.seekg(34);
    short bitDepth;
    file.read(reinterpret_cast<char*>(&bitDepth), sizeof(short));

    file.seekg(40);
    int dataSize;
    file.read(reinterpret_cast<char*>(&dataSize), sizeof(int));

    std::vector<char> data(dataSize);
    file.read(data.data(), dataSize);

    ALenum format = AL_FORMAT_MONO8;

    if(channels == 1) {

        format = (bitDepth == 16) ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8;

    } else {

        format = (bitDepth == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;

    };

    ALuint buffer, source;

    alGenBuffers(1, &buffer);

    alBufferData(buffer, format, data.data(), dataSize, sampleRate);

    alGenSources(1, &source);

    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);

    ALint state;

    do { alGetSourcei(source, AL_SOURCE_STATE, &state); } while (state == AL_PLAYING);

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// QUEUES SFX FILE
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SFXManager::QueueSFX(const std::string& filePath) {

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        queue.push(filePath);
    }

    condition.notify_one();

};

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
// ---
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- //
void SFXManager::WorkerThread() {

    while(true) {

        std::string filePath;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]() { return terminate || !queue.empty(); });

            if(terminate && queue.empty()) return;

            filePath = queue.front();
            queue.pop();
        }

        if(!terminate) PlaySound(filePath);

    };

};