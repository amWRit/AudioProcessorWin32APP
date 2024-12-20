#pragma once

#include "AudioProcessingStrategy.h"
#include "AudioFileHandler.h"
#include <memory>
#include <vector>
#include <algorithm>

class AudioReverbStrategy : public AudioProcessingStrategy {
public:
    AudioReverbStrategy(double delayTime, double decayFactor)
        : delayTime(delayTime), decayFactor(decayFactor) {
    }

    std::vector<double> process(std::shared_ptr<AudioFileHandler> fileHandler) const {
        const auto& buffer = fileHandler->getBuffer();
        std::vector<double> originalSignal = buffer[0]; // Assume mono channel for simplicity

        // Convert delayTime (in seconds) to sample count
        size_t delaySamples = static_cast<size_t>(delayTime * fileHandler->getSampleRate());

        std::vector<double> reverbSignal(originalSignal);

        // Apply the reverb effect by adding a delayed version of the signal
        for (size_t i = delaySamples; i < originalSignal.size(); ++i) {
            // Decaying the amplitude of the delayed signal
            reverbSignal[i] += originalSignal[i - delaySamples] * decayFactor;
        }

        return reverbSignal;
    }

private:
    double delayTime;   // Delay time in seconds
    double decayFactor; // Decay factor for the reverb effect (0.0 - no effect, 1.0 - same volume as original)
};
