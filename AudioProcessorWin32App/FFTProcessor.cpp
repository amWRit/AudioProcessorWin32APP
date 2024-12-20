#include "FFTProcessor.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

FFTProcessor::FFTProcessor(size_t fftSize, double sampleRate)
    : fftSize(fftSize),
    sampleRate(sampleRate),
    in(fftSize),
    out(fftSize) {
}

FFTProcessor::~FFTProcessor() {}

void FFTProcessor::performFFT(const std::vector<double>& channelData) {
    // Prepare input vector
    in.clear();
    for (size_t i = 0; i < fftSize; ++i) {
        double real = (i < channelData.size()) ? channelData[i] : 0.0;
        in.emplace_back(real, 0.0); // Fill imaginary part as 0.0
    }

    // Call FFT function
    fft(in, false);
    out = in;
}

std::vector<double> FFTProcessor::performIFFT(const std::vector<std::complex<double>>& fftOutput) {
    // Copy FFT output to input
    in = fftOutput;

    // Call IFFT function
    fft(in, true);

    // Extract real part and normalize
    std::vector<double> timeDomainData(fftSize);
    for (size_t i = 0; i < fftSize; ++i) {
        // timeDomainData[i] = in[i].real() / fftSize;
        timeDomainData[i] = in[i].real();
    }
    return timeDomainData;
}

const std::vector<std::complex<double>>& FFTProcessor::getFFTOutput() const {
    return out;
}

void FFTProcessor::fft(std::vector<std::complex<double>>& data, bool inverse) {
    // Example of Cooley-Tukey FFT algorithm (simplified, power-of-2 only)
    // just a copy/paste from genAI here; kissfft and fftw3 libraries gave problems in testing
    size_t N = data.size();
    if (N <= 1) return;

    // Bit-reversal permutation
    for (size_t i = 1, j = 0; i < N; ++i) {
        size_t bit = N >> 1;
        while (j >= bit) {
            j -= bit;
            bit >>= 1;
        }
        j += bit;
        if (i < j) std::swap(data[i], data[j]);
    }

    // FFT computation
    for (size_t len = 2; len <= N; len <<= 1) {
        double angle = 2 * M_PI / len * (inverse ? -1 : 1);
        std::complex<double> wlen(cos(angle), sin(angle));
        for (size_t i = 0; i < N; i += len) {
            std::complex<double> w(1);
            for (size_t j = 0; j < len / 2; ++j) {
                std::complex<double> u = data[i + j];
                std::complex<double> v = data[i + j + len / 2] * w;
                data[i + j] = u + v;
                data[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    // Scaling for IFFT
    if (inverse) {
        for (auto& x : data) {
            x /= N;
        }
    }
}