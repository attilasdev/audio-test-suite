#pragma once
#include <JuceHeader.h>

class FFTWrapper
{
public:
    FFTWrapper(size_t order = 11);  // Default 2048 points
    
    void prepare(double sampleRate);
    void process(const juce::AudioBuffer<float>& buffer);
    
    const std::vector<float>& getMagnitudes() const { return magnitudes; }
    float getFrequencyForBin(int bin) const;
    size_t getFFTSize() const { return fftSize; }
    
private:
    const size_t fftOrder;
    const size_t fftSize;
    
    double currentSampleRate = 44100.0;
    juce::dsp::FFT fft;
    std::vector<float> window;
    std::vector<float> fftData;
    std::vector<float> magnitudes;
    
    void applyWindow();
};