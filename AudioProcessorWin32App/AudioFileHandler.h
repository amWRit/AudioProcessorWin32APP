#pragma once
#include <vector>
#include <string>
//#include "AudioProcessingStrategy.h"

class AudioFileHandler {
public:
    // Pure virtual method for loading audio, must be implemented in derived classes
    virtual bool loadAudio(const std::string& filePath) = 0;

    // Pure virtual method for saving audio, must be implemented in derived classes
    virtual bool saveAudio(const std::string& filename,
        const std::vector<double>& timeDomainSignal,
        double sampleRate) = 0;

    // Pure virtual method for returning buffer, must be implemented in derived classes
    virtual const std::vector<std::vector<double>>& getBuffer() const = 0;

    virtual int getSampleRate() const = 0;
    virtual int getBitDepth() const = 0;
    virtual ~AudioFileHandler() = default;

};
