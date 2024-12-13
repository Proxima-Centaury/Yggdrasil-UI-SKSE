#include "../include/yggdrasil.h"

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* INITIALIZES OPENAL FOR SFX */
/* --------------------------------------------------------------------------------------------------------------------------------- */
SFXManager::SFXManager() {

    device = alcOpenDevice(nullptr);

    if(!device) {

        LogManager::Log(LogManager::LogLevel::Error, "Failed to open \"OpenAL\" device", true);

        return;

    };

    context = alcCreateContext(device, nullptr);

    if(!context || !alcMakeContextCurrent(context)) {

        LogManager::Log(LogManager::LogLevel::Error, "Failed to set \"OpenAL\" context", true);

        alcCloseDevice(device);

        return;

    };

    initialized = true;

}

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* CLEARS OPENAL RELATED DATA ON SFXMANAGER DESTRUCTION */
/* --------------------------------------------------------------------------------------------------------------------------------- */
SFXManager::~SFXManager() {

    if(context) alcDestroyContext(context);
    if(device) alcCloseDevice(device);

}

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* LOADS SFX FILE IN BUFFER */
/* --------------------------------------------------------------------------------------------------------------------------------- */
bool SFXManager::LoadSound(const std::string& filePath, ALuint& buffer) {

    std::ifstream file(filePath, std::ios::binary);

    if(!file)  {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Failed to open \"SFX\" file : \"{}\"", filePath), true);

        return false;

    };

    char chunkId[4];
    file.read(chunkId, 4);

    if(std::string(chunkId, 4) != "RIFF") {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Invalid \"SFX\" file format : \"{}\"", filePath), true);

        return false;

    };

    file.seekg(22); // Skip to channels
    short channels;
    file.read(reinterpret_cast<char*>(&channels), sizeof(short));

    file.seekg(24); // Skip to sample rate
    int sampleRate;
    file.read(reinterpret_cast<char*>(&sampleRate), sizeof(int));

    file.seekg(34); // Skip to bit depth
    short bitDepth;
    file.read(reinterpret_cast<char*>(&bitDepth), sizeof(short));

    file.seekg(40); // Skip to data chunk size
    int dataSize;
    file.read(reinterpret_cast<char*>(&dataSize), sizeof(int));

    std::vector<char> data(dataSize);
    file.read(data.data(), dataSize);

    ALenum format;

    if(channels == 1) format = (bitDepth == 16) ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8;
    if(channels != 1) format = (bitDepth == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;

    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data.data(), dataSize, sampleRate);

    return true;

};

/* --------------------------------------------------------------------------------------------------------------------------------- */
/* PLAYS SFX FILE */
/* --------------------------------------------------------------------------------------------------------------------------------- */
void SFXManager::PlaySound(const std::string& filePath) {

    ALuint buffer, source;

    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    if(LoadSound(filePath, buffer)) {

        alSourcei(source, AL_BUFFER, buffer);
        alSourcePlay(source);

        ALint state;

        do { alGetSourcei(source, AL_SOURCE_STATE, &state); } while (state == AL_PLAYING);

        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);

    } else {

        LogManager::Log(LogManager::LogLevel::Error, std::format("Failed to load \"SFX\" file : \"{}\"", filePath), true);

        return;
    
    };

};