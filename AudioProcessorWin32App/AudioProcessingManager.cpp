#include "AudioProcessingManager.h"
#include "AudioFileHandlerFactory.h" // For the factory method
#include "Utils.h"                  // For utility functions
#include <algorithm>                // For std::transform
#include <iostream>                 // For debugging/logging
#include "AudioProcessingStrategyFactory.h"

#include "framework.h"
#include <commdlg.h>  // For file dialogs

#include "Dialogs.h"

void AudioProcessingManager::setFileHandler(const std::shared_ptr<AudioFileHandler>& handler) {
    fileHandler = handler;
}

void AudioProcessingManager::setProcessedSignal(const std::vector<double>& outputSignal) {
    processedSignal = outputSignal;
}

const std::vector<double>  AudioProcessingManager::getProcessedSignal()
{
    return processedSignal;
}

std::shared_ptr<AudioFileHandler> AudioProcessingManager::loadAudioFile(const std::string& filePath) {
    // Extract file extension
    std::string format = Utils::getFileExtension(filePath);
    std::transform(format.begin(), format.end(), format.begin(), ::tolower);

    // Use factory to create the appropriate file handler
    auto audioFileHandler = AudioFileHandlerFactory::createAudioFileHandler(format);
    setFileHandler(audioFileHandler);
    if (!fileHandler) {
        std::cerr << "Unsupported audio format: " << format << std::endl;
        return nullptr;
    }

    // Load the audio file
    if (!fileHandler->loadAudio(filePath)) {
        std::cerr << "Failed to load audio file: " << filePath << std::endl;
        return nullptr;
    }

    std::cout << "Audio file loaded successfully: " << filePath << std::endl;
    return fileHandler;
}

bool AudioProcessingManager::reverseAudio() {
    if (!fileHandler) {
        OutputDebugStringA("No audio file loaded!");
        std::cerr << "No audio file loaded!" << std::endl;
        return false;
    }

    AudioProcessingStrategyFactory::ParamMap params;

    // Create the reverse audio processing strategy
    auto strategy = AudioProcessingStrategyFactory::createStrategy("audioReverse", params = {});
    if (!strategy) {
        OutputDebugStringA("Unsupported strategy!");
        std::cerr << "Unsupported strategy!" << std::endl;
        return false;
    }
    // Process the audio file and save the reversed signal

    std::vector<double> outputSignal = strategy->process(fileHandler);
    setProcessedSignal(outputSignal);
    return true;
}

bool AudioProcessingManager::reverbAudio(double delayTime, double decayFactor) {
    if (!fileHandler) {
        OutputDebugStringA("No audio file loaded!");
        std::cerr << "No audio file loaded!" << std::endl;
        return false;
    }

    AudioProcessingStrategyFactory::ParamMap params;
    params["delayTime"] = delayTime;
    params["decayFactor"] = decayFactor;

    // Create the reverse audio processing strategy
    auto strategy = AudioProcessingStrategyFactory::createStrategy("audioReverb", params);
    if (!strategy) {
        OutputDebugStringA("Unsupported strategy!");
        std::cerr << "Unsupported strategy!" << std::endl;
        return false;
    }
    // Process the audio file and save the reversed signal

    std::vector<double> outputSignal = strategy->process(fileHandler);
    setProcessedSignal(outputSignal);
    return true;
}

bool AudioProcessingManager::changeSpeed(double speedFactor) {
    if (!fileHandler) {
        OutputDebugStringA("No audio file loaded!");
        std::cerr << "No audio file loaded!" << std::endl;
        return false;
    }

    AudioProcessingStrategyFactory::ParamMap params;
    params["speedFactor"] = speedFactor;

    // Create the reverse audio processing strategy
    auto strategy = AudioProcessingStrategyFactory::createStrategy("changeAudioSpeed", params);
    if (!strategy) {
        OutputDebugStringA("Unsupported strategy!");
        std::cerr << "Unsupported strategy!" << std::endl;
        return false;
    }
    // Process the audio file and save the signal

    std::vector<double> outputSignal = strategy->process(fileHandler);
    setProcessedSignal(outputSignal);
    return true;
}

bool AudioProcessingManager::changeVolume(double volumeFactor) {
    if (!fileHandler) {
        OutputDebugStringA("No audio file loaded!");
        std::cerr << "No audio file loaded!" << std::endl;
        return false;
    }

    AudioProcessingStrategyFactory::ParamMap params;
    params["volumeFactor"] = volumeFactor;

    char buffer[256];
    sprintf_s(buffer, "%.2f", volumeFactor);  // Adjust the precision as needed
    OutputDebugStringA(buffer);

    // Create the reverse audio processing strategy
    auto strategy = AudioProcessingStrategyFactory::createStrategy("changeAudioVolume", params);
    if (!strategy) {
        OutputDebugStringA("Unsupported strategy!");
        std::cerr << "Unsupported strategy!" << std::endl;
        return false;
    }
    // Process the audio file and save the signal

    std::vector<double> outputSignal = strategy->process(fileHandler);
    setProcessedSignal(outputSignal);
    return true;
}