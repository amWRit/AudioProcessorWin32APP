#pragma once
#include <memory>
#include <iostream> 
#include <windows.h>

#include "AudioFileHandler.h"
#include "AudioProcessingStrategy.h"
//#include "ExtractAudioChunksStrategy.h"
//#include "FilterFrequencyRangeStrategy.h"
//#include "ChangeAudioSpeedStrategy.h"
//#include "ChangeAudioVolumeStrategy.h"
//#include "AudioReverbStrategy.h"
#include "AudioReverseStrategy.h"
//#include "InstrumentFactory.h"

class AudioProcessingStrategyFactory {
public:
    static std::shared_ptr<AudioProcessingStrategy> createStrategy(const std::string& strategyType, const std::string& instrumentType = "", double factor = 1.0) {
        //if (strategyType == "audioExtract") {
        //    // return std::make_shared<ExtractAudioChunksStrategy>(80.0, 5000.0);
        //    std::shared_ptr<Instrument> instrument = InstrumentFactory::createInstrument(instrumentType);
        //    if (instrument) {
        //        double lowFreq = instrument->getLowFreq();
        //        double highFreq = instrument->getHighFreq();

        //        return std::make_shared<ExtractAudioChunksStrategy>(lowFreq, highFreq);
        //    }
        //}
        //else if (strategyType == "changeAudioSpeed") {
        //    return std::make_shared<ChangeAudioSpeedStrategy>(factor);
        //}
        //else if (strategyType == "changeAudioVolume") {
        //    return std::make_shared<ChangeAudioVolumeStrategy>(factor);
        //}
        //else if (strategyType == "audioReverb") {
        //    return std::make_shared<AudioReverbStrategy>(factor, 0.5);
        //}
        if (strategyType == "audioReverse") {
            return std::make_shared<AudioReverseStrategy>();
        }
        // Add more strategy creation logic as needed
        return nullptr;
    }
};
