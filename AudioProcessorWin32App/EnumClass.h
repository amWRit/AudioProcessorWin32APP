#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>

//enums for StrategyType
enum class StrategyType {
    AudioExtract,
    ChangeAudioSpeed,
    ChangeAudioVolume,
    AudioReverb,
    AudioReverse,
    Unknown
};

const std::unordered_map<StrategyType, std::string> strategyTypeToString = {
    {StrategyType::AudioExtract, "AudioExtract"},
    {StrategyType::ChangeAudioSpeed, "changeAudioSpeed"},
    {StrategyType::ChangeAudioVolume, "changeAudioVolume"},
    {StrategyType::AudioReverb, "audioReverb"},
    {StrategyType::AudioReverse, "audioReverse"},
    {StrategyType::Unknown, "Unknown"},
};

inline std::string toString(StrategyType strategyType) {
    auto it = strategyTypeToString.find(strategyType);
    return (it != strategyTypeToString.end()) ? it->second : "Unknown";
}

//enums for InstrumentType
enum class InstrumentType {
    Guitar,
    Drum,
    Piano,
    XXHuman,
    XYHuman,
    Cymbal,
    Synth,
    Unknown
};

const std::unordered_map<InstrumentType, std::string> instrumentTypeToString = {
    {InstrumentType::Guitar, "Guitar"},
    {InstrumentType::Drum, "Drum"},
    {InstrumentType::Piano, "Piano"},
    {InstrumentType::XXHuman, "XXHuman"},
    {InstrumentType::XYHuman, "XYHuman"},
    {InstrumentType::Cymbal, "Cymbal"},
    {InstrumentType::Synth, "Synth"},
    {InstrumentType::Unknown, "Unknown"},
};

inline std::string toString(InstrumentType instrumentType) {
    auto it = instrumentTypeToString.find(instrumentType);
    return (it != instrumentTypeToString.end()) ? it->second : "Unknown";
}