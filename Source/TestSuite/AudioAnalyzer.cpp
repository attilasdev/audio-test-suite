#include "AudioAnalyzer.h"

AudioAnalyzer::AudioAnalyzer()
    : fft(fftOrder)
    , fftData(fftSize * 2, 0.0f)
    , window(fftSize, 0.0f)
    , currentSampleRate(44100.0)
{
    // Initialize Hanning window
    juce::dsp::WindowingFunction<float>::fillWindowingTables(window.data(), fftSize, 
        juce::dsp::WindowingFunction<float>::hann, false);
        
    // Initialize spectrum with zeros
    lastResult.spectrum.resize(fftSize / 2, 0.0f);
}

void AudioAnalyzer::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    lastResult.spectrum.resize(fftSize / 2, 0.0f);
}

void AudioAnalyzer::analyze(const juce::AudioBuffer<float>& buffer)
{
    lastResult.rms = calculateRMS(buffer);
    lastResult.peak = findPeakLevel(buffer);
    performFFT(buffer);
    lastResult.fundamentalFrequency = findFundamentalFrequency();
}

float AudioAnalyzer::calculateRMS(const juce::AudioBuffer<float>& buffer)
{
    float sum = 0.0f;
    const auto numChannels = buffer.getNumChannels();
    const auto numSamples = buffer.getNumSamples();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        for (int i = 0; i < numSamples; ++i)
        {
            sum += channelData[i] * channelData[i];
        }
    }
    
    return std::sqrt(sum / (numChannels * numSamples));
}

float AudioAnalyzer::findPeakLevel(const juce::AudioBuffer<float>& buffer)
{
    float peak = 0.0f;
    const auto numChannels = buffer.getNumChannels();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            peak = std::max(peak, std::abs(channelData[i]));
        }
    }
    
    return peak;
}

void AudioAnalyzer::performFFT(const juce::AudioBuffer<float>& buffer)
{
    // Copy audio data to FFT buffer
    const float* channelData = buffer.getReadPointer(0);
    const auto numSamples = std::min(static_cast<int>(fftSize), buffer.getNumSamples());
    
    // Apply window and copy to FFT buffer
    for (int i = 0; i < numSamples; ++i)
    {
        fftData[i] = channelData[i] * window[i];
    }
    
    // Zero-pad if needed
    for (size_t i = numSamples; i < fftSize; ++i)
    {
        fftData[i] = 0.0f;
    }
    
    // Perform FFT
    fft.performFrequencyOnlyForwardTransform(fftData.data());
    
    // Update spectrum with smoothing
    const float smoothingFactor = 0.7f;
    for (size_t i = 0; i < fftSize / 2; ++i)
    {
        lastResult.spectrum[i] = lastResult.spectrum[i] * smoothingFactor + 
                                fftData[i] * (1.0f - smoothingFactor);
    }
}

float AudioAnalyzer::findFundamentalFrequency()
{
    // Simple peak finding in the lower frequency range
    size_t maxBin = 0;
    float maxValue = 0.0f;
    
    // Look at lower frequency range (up to 1000 Hz)
    size_t upperBin = static_cast<size_t>((1000.0f * fftSize) / currentSampleRate);
    
    for (size_t i = 1; i < upperBin; ++i)
    {
        if (lastResult.spectrum[i] > maxValue)
        {
            maxValue = lastResult.spectrum[i];
            maxBin = i;
        }
    }
    
    return (maxBin * currentSampleRate) / fftSize;
}