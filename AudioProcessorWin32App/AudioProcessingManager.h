#pragma once
#ifndef AUDIO_PROCESSOR_MANAGER_H
#define AUDIO_PROCESSOR_MANAGER_H

#include <memory>
#include <string>
#include "AudioFileHandler.h" // Include the necessary header for AudioFileHandler

class AudioProcessingManager {
    public:
        void setFileHandler(const std::shared_ptr<AudioFileHandler>& handler);
        void setProcessedSignal(const std::vector<double>& outputSignal);
        const std::vector<double> getProcessedSignal();
        // Loads an audio file and returns a shared pointer to the AudioFileHandler.
        std::shared_ptr<AudioFileHandler> loadAudioFile(const std::string& filePath);

        bool reverseAudio();
    private:
        std::shared_ptr<AudioFileHandler> fileHandler; // Store the audio file handler
        std::vector<double> processedSignal;
};

#endif // AUDIO_PROCESSOR_MANAGER_H
