#pragma once
#ifndef INSTRUMENT_FACTORY_H
#define INSTRUMENT_FACTORY_H

#include <memory>
#include <string>
#include "Instrument.h"
#include "EnumClass.h"

class InstrumentFactory {
public:
    static std::shared_ptr<Instrument> createInstrument(InstrumentType instrumentType);
};

#endif // INSTRUMENT_Hs