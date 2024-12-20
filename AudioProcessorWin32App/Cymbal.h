#include "Instrument.h"

class Cymbal : public Instrument {
public:
    double getLowFreq() const override { return 5000.0; }
    double getHighFreq() const override { return 10000.0; }
};
