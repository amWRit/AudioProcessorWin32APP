#pragma once
#include <memory>
#include <iostream> 
#include <windows.h>
#include <map>
#include <string>
#include <variant>
#include <unordered_map>


#include "AudioFileHandler.h"
#include "AudioProcessingStrategy.h"
//#include "ExtractAudioChunksStrategy.h"
//#include "FilterFrequencyRangeStrategy.h"
#include "ChangeAudioSpeedStrategy.h"
#include "ChangeAudioVolumeStrategy.h"
#include "AudioReverbStrategy.h"
#include "AudioReverseStrategy.h"
//#include "InstrumentFactory.h"

class AudioProcessingStrategyFactory {
public:
    using ParamMap = std::unordered_map<std::string, std::variant<std::string, double, int>>;

    static std::shared_ptr<AudioProcessingStrategy> createStrategy(
        const std::string& strategyType, const ParamMap& params = {}) {

        if (strategyType == "audioExtract") {
            double lowFreq = std::get<double>(params.at("lowFreq"));
            double highFreq = std::get<double>(params.at("highFreq"));
            //return std::make_shared<ExtractAudioChunksStrategy>(lowFreq, highFreq);
        }
        else if (strategyType == "changeAudioSpeed") {
            double speedFactor = std::get<double>(params.at("speedFactor"));
            return std::make_shared<ChangeAudioSpeedStrategy>(speedFactor);
        }
        else if (strategyType == "changeAudioVolume") {
            double volumeFactor = std::get<double>(params.at("volumeFactor"));
            return std::make_shared<ChangeAudioVolumeStrategy>(volumeFactor);
        }
        else if (strategyType == "audioReverb") {
            double delayTime = std::get<double>(params.at("delayTime"));
            double decayFactor = std::get<double>(params.at("decayFactor"));
            return std::make_shared<AudioReverbStrategy>(delayTime, decayFactor);
        }
        else if (strategyType == "audioReverse") {
            return std::make_shared<AudioReverseStrategy>();
        }

        // Add more strategies as needed
        return nullptr;
    }
};
