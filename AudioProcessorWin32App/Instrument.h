#pragma once
#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <memory>

class Instrument {
public:
    virtual double getLowFreq() const = 0;
    virtual double getHighFreq() const = 0;
    virtual ~Instrument() = default;
};

#endif // INSTRUMENT_H