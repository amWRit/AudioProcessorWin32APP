#pragma once
#include "AudioProcessingStrategy.h"
//#include "SignalProcessor.h"
#include "AudioFileHandler.h"
#include <memory>

class ChangeAudioSpeedStrategy : public AudioProcessingStrategy {
public:
    ChangeAudioSpeedStrategy(double speedFactor)
        : speedFactor(speedFactor) {
    }

    std::vector<double> process(std::shared_ptr<AudioFileHandler> fileHandler) const {
        const auto& buffer = fileHandler->getBuffer();
        std::vector<double> originalSignal = buffer[0];
        double sampleRate = fileHandler->getSampleRate();
        int newSize = static_cast<int>(originalSignal.size() / speedFactor);

        std::vector<double> processedSignal(newSize);
        for (int i = 0; i < newSize; ++i) {
            int index = static_cast<int>(i * speedFactor);
            if (index < originalSignal.size()) {
                processedSignal[i] = originalSignal[index];
            }
        }
        return processedSignal;
    }

private:
    double speedFactor; // Factor to speed up the audio
};
