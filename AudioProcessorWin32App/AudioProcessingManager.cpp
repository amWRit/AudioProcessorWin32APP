#include "AudioProcessingManager.h"
#include "AudioFileHandlerFactory.h" // For the factory method
#include "Utils.h"                  // For utility functions
#include <algorithm>                // For std::transform
#include <iostream>                 // For debugging/logging
#include <string>
#include <cstdio>
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
    return processAudio({});
}

bool AudioProcessingManager::reverbAudio(double delayTime, double decayFactor) {
    return processAudio({ {"delayTime", delayTime}, {"decayFactor", decayFactor} });
}

bool AudioProcessingManager::changeSpeed(double speedFactor) {
    return processAudio({ {"speedFactor", speedFactor} });
}

bool AudioProcessingManager::changeVolume(double volumeFactor) {
    //== Testing ==
    //char buffer[256];
    //sprintf_s(buffer, "%.2f", volumeFactor);
    //OutputDebugStringA((std::string("Volume Factor: ") + buffer + "\n").c_str());

    return processAudio({ {"volumeFactor", volumeFactor} });
}

bool AudioProcessingManager::extractAudio(InstrumentType instrumentType) {
    auto instrument = InstrumentFactory::createInstrument(instrumentType);
    if (!instrument) {
        OutputDebugStringA("Unsupported instrument type!\n");
        std::cerr << "Unsupported instrument type!" << std::endl;
        return false;
    }

    AudioProcessingStrategyFactory::ParamMap params = {
        {"lowFreq", instrument->getLowFreq()},
        {"highFreq", instrument->getHighFreq()}
    };

    //== Testing == 
    //char buffer[64];
    //sprintf_s(buffer, "Low Frequency: %f\n", std::get<double>(params["lowFreq"]));
    //OutputDebugStringA(buffer);

    //sprintf_s(buffer, "High Frequency: %f\n", std::get<double>(params["highFreq"]));
    //OutputDebugStringA(buffer);

    return processAudio(params);
}

bool AudioProcessingManager::processAudio(const AudioProcessingStrategyFactory::ParamMap& params) {
    if (!fileHandler) {
        OutputDebugStringA("No audio file loaded!\n");
        std::cerr << "No audio file loaded!" << std::endl;
        return false;
    }

    auto strategy = AudioProcessingStrategyFactory::createStrategy(strategyType, params);
    if (!strategy) {
        OutputDebugStringA("Unsupported strategy!\n");
        std::cerr << "Unsupported strategy!" << std::endl;
        return false;
    }

    setProcessedSignal(strategy->process(fileHandler));
    return true;
}