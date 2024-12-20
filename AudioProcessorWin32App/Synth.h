#include "Instrument.h"

class Synth : public Instrument {
public:
    double getLowFreq() const override { return 10000.0; }
    double getHighFreq() const override { return 20000.0; }
};
