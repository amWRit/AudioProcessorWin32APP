#pragma once
#include "AudioProcessingStrategy.h"
#include "AudioFileHandler.h"
#include <memory>
#include <vector>
#include <algorithm>

class AudioReverseStrategy : public AudioProcessingStrategy {
public:
    AudioReverseStrategy() = default;

    std::vector<double> process(std::shared_ptr<AudioFileHandler> fileHandler) const {
        const auto& buffer = fileHandler->getBuffer();
        std::vector<double> originalSignal = buffer[0]; // Assume mono channel for simplicity

        // Reverse the audio signal
        std::reverse(originalSignal.begin(), originalSignal.end());
        return originalSignal;
    }
};
