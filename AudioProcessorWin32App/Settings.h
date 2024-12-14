#pragma once

#include <cstddef>

class Settings {
public:
    static int getFFTSize() { return fftSize; }
    static void setFFTSize(size_t size) { fftSize = size; }

private:
    static size_t fftSize; // Declare as extern
};
