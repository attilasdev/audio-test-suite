#pragma once
#include <JuceHeader.h>

class SignalGenerator
{
public:
    enum class WaveType
    {
        Sine,
        Square,
        WhiteNoise,
        Off
    };

    SignalGenerator();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setFrequency(float freqHz);
    void setWaveType(WaveType type);
    void setAmplitude(float amp);
    
    void process(juce::AudioBuffer<float>& buffer);
    bool isActive() const { return currentType != WaveType::Off; }
    
private:
    double currentSampleRate = 44100.0;
    float frequency = 440.0f;
    float amplitude = 0.5f;
    WaveType currentType = WaveType::Off;
    
    double phase = 0.0;
    const double twoPi = 2.0 * juce::MathConstants<double>::pi;
    
    juce::Random random;
    juce::dsp::Oscillator<float> oscillator;
    
    float generateSample();
};