#pragma once
#include "AudioProcessingStrategy.h"
//#include "SignalProcessor.h"
#include "AudioFileHandler.h"
#include <memory>

class ChangeAudioVolumeStrategy : public AudioProcessingStrategy {
public:
    ChangeAudioVolumeStrategy(double volumeFactor)
        : volumeFactor(volumeFactor) {
    }

    std::vector<double> process(std::shared_ptr<AudioFileHandler> fileHandler) const {
        const auto& buffer = fileHandler->getBuffer();
        std::vector<double> originalSignal = buffer[0];
        for (auto& sample : originalSignal) {
            sample *= volumeFactor;
        }

        return originalSignal;
    }
private:
    double volumeFactor; // Factor to change the volume
};