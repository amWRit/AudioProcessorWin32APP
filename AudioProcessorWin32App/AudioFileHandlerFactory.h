#pragma once
#include <memory>
#include "AudioFileHandler.h"
#include "WaveAudioFileHandler.h"

class AudioFileHandlerFactory {
public:
    static std::shared_ptr<AudioFileHandler> createAudioFileHandler(const std::string& type) {
        if (type == "wav" || type == "aiff") {
            return std::make_shared<WaveAudioFileHandler>();
        }

        // Todo: Add additional formats.
        // if (type == "mp3") {
        //     return std::make_unique<MP3AudioProcessor>();
        // }
        return nullptr;
    }
};
