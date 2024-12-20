#include <iostream>
#include <vector>
#include <algorithm>
#include "Settings.h"
#include "SignalProcessor.h"
#include "FFTProcessor.h"
#include <complex>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Utility method to filter frequencies within a given range
std::vector<double> filterFrequencies(const std::vector<double>& frequencies,
    const std::vector<double>& magnitudes,
    double lowRange,
    double highRange,
    double magnitudeThreshold) {
    std::vector<double> filteredFrequencies;

    for (size_t i = 0; i < frequencies.size(); ++i) {
        if (magnitudes[i] > magnitudeThreshold) { // Suppress noise
            if (frequencies[i] >= lowRange && frequencies[i] <= highRange) {
                filteredFrequencies.push_back(frequencies[i]);
            }
        }
    }

    return filteredFrequencies;
}

// Helper method to filter FFT Output within a given range
std::vector<std::complex<double>> SignalProcessor::filterFFTOutput(
    const std::vector<std::complex<double>>& fftOutput,
    const std::vector<double>& frequencies,
    double lowFreq, double highFreq) {

    std::vector<std::complex<double>> filteredFFTOutput(fftOutput.size());
    for (size_t i = 0; i < frequencies.size(); ++i) {
        if (frequencies[i] >= lowFreq && frequencies[i] <= highFreq) {
            filteredFFTOutput[i] = fftOutput[i];
            // std::cout << "Keeping frequency: " << frequencies[i] << std::endl;
        }
        else {
            filteredFFTOutput[i] = std::complex<double>(0.0, 0.0);
            // Apply windowing (e.g., Hamming) outside the range to reduce sharp cutoff artifacts
            // double window = 0.5 * (1 - cos(2 * M_PI * (frequencies[i] - lowFreq) / (highFreq - lowFreq))); // Hamming window
            // filteredFFTOutput[i] = fftOutput[i] * window;
        }
    }
    return filteredFFTOutput;
}

// Helper function to compare two signals
// Not sure if it's working or not
bool SignalProcessor::compareSignals(
    const std::vector<double>& original,
    const std::vector<double>& processed,
    double tolerance) {
    if (original.size() != processed.size()) {
        std::cerr << "Signal sizes do not match!" << std::endl;
        return false;
    }

    for (size_t i = 0; i < original.size(); ++i) {
        if (std::abs(original[i] - processed[i]) > tolerance) {
            std::cerr << "Signals differ at index " << i << ": "
                << original[i] << " vs " << processed[i] << std::endl;
            return false;
        }
    }

    return true;
}

std::vector<double> SignalProcessor::calculateFrequencies(
    size_t fftSize,
    double sampleRate,
    const std::vector<std::complex<double>>& fftOutput) {
    std::vector<double> frequencies;
    for (size_t i = 0; i < fftSize / 2; ++i) { // Only up to Nyquist frequency
        double frequency = static_cast<double>(i) * sampleRate / fftSize; //
        frequencies.push_back(frequency);
    }
    return frequencies;
}

std::vector<double> SignalProcessor::calculateMagnitudes(
    size_t fftSize,
    double sampleRate,
    const std::vector<std::complex<double>>& fftOutput) {
    std::vector<double> magnitudes;
    for (size_t i = 0; i < fftSize / 2; ++i) { // Only up to Nyquist frequency
        double magnitude = std::sqrt(fftOutput[i].real() * fftOutput[i].real() + fftOutput[i].imag() * fftOutput[i].imag());
        magnitudes.push_back(magnitude);
    }
    return magnitudes;
}

// method for chunking, processing and reconstructing audio
// given a signal, in chunks of fftsize, performs fft, gets frequencies,
// filters frequencies within a range, performs ifft, then reconstructs into filtered signal
std::vector<double> SignalProcessor::extractAudioFromChunks(
    const std::vector<double>& originalSignal,
    double sampleRate,
    double lowFreq,
    double highFreq) {
    size_t signalSize = originalSignal.size();
    std::vector<double> extractedSignal;
    size_t fftSize = Settings::getFFTSize();
    size_t numChunks = signalSize / fftSize;

    // std::cout << "Extraction started...:\n";

    // std::cout << "Sample Rate: " << sampleRate << " Hz\n";
    // std::cout << "Size: " << originalSignal.size() << "\n";

    // Process each chunk of fftSize
    for (size_t i = 0; i < numChunks; ++i) {
        std::vector<double> chunk(originalSignal.begin() + i * fftSize,
            originalSignal.begin() + (i + 1) * fftSize);
        FFTProcessor fftProcessor(fftSize, sampleRate);
        fftProcessor.performFFT(chunk);
        const std::vector<std::complex<double>>& fftOutput = fftProcessor.getFFTOutput();
        std::vector<double> frequencies = calculateFrequencies(fftSize, sampleRate, fftOutput);

        std::vector<std::complex<double>> filteredFFTOutput = filterFFTOutput(fftOutput, frequencies, lowFreq, highFreq);

        // std::cout << "Filtered fft output:\n";
        // for (size_t i = 0; i < filteredFFTOutput.size(); ++i) {
        //     std::cout << "fft output: " << filteredFFTOutput[i] << "\n";
        // }

        // Convert filteredFFTOutput (std::vector<std::complex<double>>) to std::vector<kiss_fft_cpx>
        //std::vector<kiss_fft_cpx> kissFilteredFFTOutput = fftProcessor.convertToKissFFTFormat(filteredFFTOutput);

        std::vector<double> ifftOutput = fftProcessor.performIFFT(filteredFFTOutput);  // Call the existing IFFT method

        // For printing purposes
        // std::cout << "Filtered ifft output:\n";
        // for (size_t i = 0; i < ifftOutput.size(); ++i) {
        //     std::cout << "ifft output: " << ifftOutput[i] << "\n";
        // }

        extractedSignal.insert(extractedSignal.end(), ifftOutput.begin(), ifftOutput.end());
    }

    return extractedSignal;
}