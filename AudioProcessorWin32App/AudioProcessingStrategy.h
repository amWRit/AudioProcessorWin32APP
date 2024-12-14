#pragma once
// AudioProcessingStrategy.h
#pragma once
#include <vector>
#include <memory>
#include "AudioFileHandler.h"

class AudioFileHandler;

class AudioProcessingStrategy {
public:
    virtual ~AudioProcessingStrategy() = default;

    // Pure virtual function to process audio data
    virtual std::vector<double> process(std::shared_ptr<AudioFileHandler> fileHandler) const = 0;
};
