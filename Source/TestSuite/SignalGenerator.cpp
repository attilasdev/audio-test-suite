#include "SignalGenerator.h"

SignalGenerator::SignalGenerator()
{
    oscillator.initialise([](float x) { return std::sin(x); });
}

void SignalGenerator::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    phase = 0.0;
    oscillator.prepare(spec);
}

void SignalGenerator::setFrequency(float freqHz)
{
    frequency = freqHz;
    oscillator.setFrequency(frequency);
}

void SignalGenerator::setWaveType(WaveType type)
{
    currentType = type;
    
    switch (type)
    {
        case WaveType::Sine:
            oscillator.initialise([](float x) { return std::sin(x); });
            break;
            
        case WaveType::Square:
            oscillator.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
            
        default:
            break;
    }
}

void SignalGenerator::setAmplitude(float amp)
{
    amplitude = amp;
}

float SignalGenerator::generateSample()
{
    switch (currentType)
    {
        case WaveType::Sine:
        case WaveType::Square:
            return oscillator.processSample(0.0f) * amplitude;
            
        case WaveType::WhiteNoise:
            return (random.nextFloat() * 2.0f - 1.0f) * amplitude;
            
        case WaveType::Off:
        default:
            return 0.0f;
    }
}

void SignalGenerator::process(juce::AudioBuffer<float>& buffer)
{
    if (currentType == WaveType::Off)
    {
        buffer.clear();
        return;
    }

    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();
    
    // Process first channel more efficiently
    float* writePointer = buffer.getWritePointer(0);
    
    if (currentType == WaveType::WhiteNoise)
    {
        for (int i = 0; i < numSamples; ++i)
            writePointer[i] = (random.nextFloat() * 2.0f - 1.0f) * amplitude;
    }
    else
    {
        // Use JUCE's optimized oscillator for Sine and Square
        for (int i = 0; i < numSamples; ++i)
            writePointer[i] = oscillator.processSample(0.0f) * amplitude;
    }

    // Copy to other channels efficiently
    for (int channel = 1; channel < numChannels; ++channel)
        buffer.copyFrom(channel, 0, buffer, 0, 0, numSamples);
}