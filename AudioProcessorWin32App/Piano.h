#include "Instrument.h"

class Piano : public Instrument {
public:
    double getLowFreq() const override { return 28.0; }
    double getHighFreq() const override { return 4000.0; }
};
