#include "InstrumentFactory.h"
#include "Guitar.h"
#include "Drum.h"
#include "Piano.h"
#include "XXHuman.h"
#include "XYHuman.h"
#include "Cymbal.h"
#include "Synth.h"
// #include "../include/EnumClass.h"

std::shared_ptr<Instrument> InstrumentFactory::createInstrument(InstrumentType instrumentType) {
    if (instrumentType == InstrumentType::Guitar) return std::make_shared<Guitar>();
    if (instrumentType == InstrumentType::Drum) return std::make_shared<Drum>();
    if (instrumentType == InstrumentType::Piano) return std::make_shared<Piano>();
    if (instrumentType == InstrumentType::XXHuman) return std::make_shared<XXHuman>();
    if (instrumentType == InstrumentType::XYHuman) return std::make_shared<XYHuman>();
    if (instrumentType == InstrumentType::Cymbal) return std::make_shared<Cymbal>();
    if (instrumentType == InstrumentType::Synth) return std::make_shared<Synth>();
    return nullptr; // Or handle unknown instrument types
}
