#pragma once

#include <JuceHeader.h>
#include <vector>

class AudioAnalyzer
{
public:
    struct AnalysisResult
    {
        float rms;
        float peak;
        float fundamentalFrequency;
        std::vector<float> spectrum;
    };

    AudioAnalyzer();
    void prepare(const juce::dsp::ProcessSpec& spec);
    void analyze(const juce::AudioBuffer<float>& buffer);

private:
    float calculateRMS(const juce::AudioBuffer<float>& buffer);
    float findPeakLevel(const juce::AudioBuffer<float>& buffer);
    void performFFT(const juce::AudioBuffer<float>& buffer);
    float findFundamentalFrequency();

    static constexpr size_t fftOrder = 11;  // for 2048 points
    static constexpr size_t fftSize = 1 << fftOrder;
    
    juce::dsp::FFT fft;
    std::vector<float> fftData;
    std::vector<float> window;
    double currentSampleRate;
    AnalysisResult lastResult;
};