#include "Instrument.h"

class XYHuman : public Instrument {
public:
    double getLowFreq() const override { return 85.0; }
    double getHighFreq() const override { return 180.0; }
};
