#include "Instrument.h"

class Drum : public Instrument {
public:
    double getLowFreq() const override { return 40.0; }
    double getHighFreq() const override { return 120.0; }
};
