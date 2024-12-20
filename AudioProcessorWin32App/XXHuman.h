#include "Instrument.h"

class XXHuman : public Instrument {
public:
    double getLowFreq() const override { return 165.0; }
    double getHighFreq() const override { return 300.0; }
};
