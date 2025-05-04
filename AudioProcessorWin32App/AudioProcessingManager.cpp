#pragma once
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

#include <onnxruntime_cxx_api.h>
#include <onnxruntime_c_api.h>
//#include <onnxruntime_cxx_inline.h>
#include <onnxruntime_float16.h>

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

std::string AudioProcessingManager::detectInstrument() {
    const auto& buffer = fileHandler->getBuffer();
    std::vector<double> rawWaveform = buffer[0];

    // If values aren't normalized:
    double maxAmplitude = *std::max_element(rawWaveform.begin(), rawWaveform.end());
    std::transform(rawWaveform.begin(), rawWaveform.end(), rawWaveform.begin(),
        [maxAmplitude](double sample) { return sample / maxAmplitude; });

    //Padding
    const int targetLength = 16000; // 1 second at 16 kHz
    if (rawWaveform.size() < targetLength) {
        rawWaveform.resize(targetLength, 0.0); // Pad with zeros
    }
    else if (rawWaveform.size() > targetLength) {
        rawWaveform = std::vector<double>(rawWaveform.begin(), rawWaveform.begin() + targetLength); // Trim
    }

    // Convert to float for ONNX
    std::vector<float> inputTensorValues(rawWaveform.begin(), rawWaveform.end());

    // ONNX Runtime setup
    //Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "InstrumentDetection");
    Ort::Env env;
    Ort::SessionOptions sessionOptions;
    //Ort::Session session(env, L"model.onnx", sessionOptions);
    
    Ort::Session session{ env, L"model.onnx", Ort::SessionOptions{nullptr} };
    //try {
    //    Ort::Session session(env, L"model.onnx", sessionOptions);
    //}
    //catch (const Ort::Exception& e) {
    //    std::cerr << "ONNX Runtime error: " << e.what() << std::endl;
    //    return "";
    //}

    // Define input shape and memory info
    std::vector<int64_t> inputShape = { 1, static_cast<int64_t>(inputTensorValues.size()) };
    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    // Create input tensor
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(memoryInfo, inputTensorValues.data(),
        inputTensorValues.size(), inputShape.data(), inputShape.size());

    // Run inference
    const char* inputNames[] = { "input" };
    const char* outputNames[] = { "output" };
    auto outputTensor = session.Run(Ort::RunOptions{ nullptr }, inputNames, &inputTensor, 1, outputNames, 1);

    // Extract output
    float* outputData = outputTensor[0].GetTensorMutableData<float>();
    int instrumentIndex = std::max_element(outputData, outputData + 4) - outputData;

    // Map index to instrument name
    std::vector<std::string> instrumentNames = { "Guitar", "Piano", "Drums", "Violin" };
    return instrumentNames[instrumentIndex];

    //return "Guitar";
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