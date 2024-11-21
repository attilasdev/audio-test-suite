#include "FFTWrapper.h"

FFTWrapper::FFTWrapper(size_t order)
    : fftOrder(order)
    , fftSize(1 << order)
    , fft(order)
    , window(fftSize)
    , fftData(fftSize * 2)
    , magnitudes(fftSize / 2)
{
    // Create Hanning window
    for (size_t i = 0; i < fftSize; ++i)
        window[i] = 0.5f - 0.5f * std::cos(2.0f * juce::MathConstants<float>::pi * i / (fftSize - 1));
}

void FFTWrapper::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
}

void FFTWrapper::process(const juce::AudioBuffer<float>& buffer)
{
    const auto* channelData = buffer.getReadPointer(0);
    const auto numSamples = std::min(static_cast<size_t>(buffer.getNumSamples()), fftSize);
    
    // Use SIMD operations where possible
    juce::FloatVectorOperations::multiply(fftData.data(), channelData, window.data(), numSamples);
    std::fill(fftData.begin() + numSamples, fftData.end(), 0.0f);
    
    fft.performFrequencyOnlyForwardTransform(fftData.data());
    
    // Update magnitudes
    juce::FloatVectorOperations::copy(magnitudes.data(), fftData.data(), fftSize / 2);
}

float FFTWrapper::getFrequencyForBin(int bin) const
{
    return bin * currentSampleRate / fftSize;
}