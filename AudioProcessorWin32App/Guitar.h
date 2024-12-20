#include "Instrument.h"

class Guitar : public Instrument {
public:
    double getLowFreq() const override { return 80.0; }
    double getHighFreq() const override { return 5000.0; }
};
