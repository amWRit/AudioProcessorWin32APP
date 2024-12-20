// ExtractAudioChunksStrategy.h
#pragma once
#include "AudioProcessingStrategy.h"
#include "SignalProcessor.h"
#include "AudioFileHandler.h"
// #include "Instrument.h"
#include <memory>

class ExtractAudioChunksStrategy : public AudioProcessingStrategy {
public:
    ExtractAudioChunksStrategy(double minFreq, double maxFreq)
        : minFreq(minFreq), maxFreq(maxFreq) {
    }

    // std::vector<double> process(AudioFileHandler& fileHandler) const override {
    //     // Convert shared_ptr to raw pointer and pass as reference
    //     return process(*fileHandler);  // Dereference shared_ptr to pass as reference
    // }

    std::vector<double> process(std::shared_ptr<AudioFileHandler> fileHandler) const {
        // Access buffer and metadata through AudioFileHandler
        const auto& buffer = fileHandler->getBuffer();
        double sampleRate = fileHandler->getSampleRate();

        std::vector<double> timeDomainSignal = SignalProcessor::extractAudioFromChunks(buffer[0], sampleRate,
            minFreq, maxFreq);
        return timeDomainSignal;
    }
private:
    double minFreq;
    double maxFreq;
};
