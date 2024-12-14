#pragma once

#include <windows.h>
#include <iostream>

#include "AudioFileHandler.h"
#include "AudioFile.h"

class WaveAudioFileHandler : public AudioFileHandler {
private:
    AudioFile<double> audioFile;
    std::vector<std::vector<double>> buffer;
public:

    bool loadAudio(const std::string& filePath) override {
        if (!audioFile.load(filePath)) {
            return false;
        }

        // Get audio data into buffer
        buffer = audioFile.samples;
        return true;
    }

    // Save audio to a file (since it's a small method, we can keep it here)
    bool saveAudio(const std::string& filename, const std::vector<double>& timeDomainSignal, double sampleRate) {
        AudioFile<double> audioFile;
        AudioFile<double>::AudioBuffer buffer(1, timeDomainSignal); // Mono
        audioFile.setAudioBuffer(buffer);
        audioFile.setSampleRate(static_cast<int>(sampleRate));
        audioFile.save(filename);
        return true;
    }

    const std::vector<std::vector<double>>& getBuffer() const override {
        return buffer;
    }

    int getSampleRate() const override {
        return audioFile.getSampleRate();
    }

    int getBitDepth() const override {
        return audioFile.getBitDepth();
    }
};
