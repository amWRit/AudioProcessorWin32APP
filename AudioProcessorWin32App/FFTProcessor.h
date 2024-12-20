#ifndef FFTPROCESSOR_H
#define FFTPROCESSOR_H

#include <vector>
#include <complex>

class FFTProcessor {
public:
    FFTProcessor(size_t fftSize, double sampleRate);
    ~FFTProcessor();

    void performFFT(const std::vector<double>& channelData);
    std::vector<double> performIFFT(const std::vector<std::complex<double>>& fftOutput);
    const std::vector<std::complex<double>>& getFFTOutput() const;

private:
    size_t fftSize;
    double sampleRate;
    std::vector<std::complex<double>> in, out;

    // Helper methods for FFT/IFFT 
    void fft(std::vector<std::complex<double>>& data, bool inverse);
};

#endif // FFTPROCESSOR_H