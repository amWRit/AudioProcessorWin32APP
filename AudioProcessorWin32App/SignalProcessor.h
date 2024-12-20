#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

#include <vector>
#include <functional>
#include "FFTProcessor.h"

// Todo: for now all the methods here are static
// this could be combined with audioFileHandler or use singleton audioProcessingManager?
class SignalProcessor {
public:
    // Utility method to filter frequencies within a given range
    std::vector<double> filterFrequencies(const std::vector<double>& frequencies,
        const std::vector<double>& magnitudes,
        double lowRange,
        double highRange,
        double magnitudeThreshold);

    // Helper function to filter FFT Output within a given range
    static std::vector<std::complex<double>> filterFFTOutput(
        const std::vector<std::complex<double>>& fftOutput,
        const std::vector<double>& frequencies,
        double lowFreq, double highFreq);

    // Helper function to compare two signals
    static bool compareSignals(
        const std::vector<double>& original,
        const std::vector<double>& processed,
        double tolerance = 1e-6);

    // return frequencies from fftouput
    static std::vector<double> calculateFrequencies(
        size_t fftSize,
        double sampleRate,
        const std::vector<std::complex<double>>& fftOutput
    );

    // return magnittudes from fftouput
    static std::vector<double> calculateMagnitudes(
        size_t fftSize,
        double sampleRate,
        const std::vector<std::complex<double>>& fftOutput
    );

    // method for chunking, processing and reconstructing audio
    static std::vector<double> extractAudioFromChunks(
        const std::vector<double>& originalSignal,
        double sampleRate,
        double lowFreq,
        double highFreq
    );

};

#endif // SIGNALPROCESSOR_H