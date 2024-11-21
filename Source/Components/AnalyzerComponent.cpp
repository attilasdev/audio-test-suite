#include "AnalyzerComponent.h"

AnalyzerComponent::AnalyzerComponent(AudioAnalyzer& analyzer)
    : audioAnalyzer(analyzer)
{
    startTimerHz(TIMER_HZ);
}

void AnalyzerComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(4);
    auto spectrumBounds = bounds.removeFromTop(bounds.getHeight() * 2/3);
    auto meterBounds = bounds;

    // Draw spectrum
    g.setColour(juce::Colours::black);
    g.fillRect(spectrumBounds);
    drawSpectrum(g, spectrumBounds);

    // Draw meters
    auto meterWidth = meterBounds.getWidth() / 2;
    drawMeter(g, meterBounds.removeFromLeft(meterWidth).reduced(2), smoothedRMS);
    drawMeter(g, meterBounds.reduced(2), smoothedPeak);
}

void AnalyzerComponent::drawSpectrum(juce::Graphics& g, const juce::Rectangle<int>& bounds)
{
    const auto& spectrum = audioAnalyzer.getLastResult().spectrum;
    if (spectrum.empty()) return;

    juce::Path p;
    float width = bounds.getWidth();
    float height = bounds.getHeight();
    float binWidth = width / spectrum.size();

    p.startNewSubPath(bounds.getX(), bounds.getBottom());

    for (size_t i = 0; i < spectrum.size(); ++i)
    {
        float x = bounds.getX() + i * binWidth;
        float level = juce::jmap(spectrum[i], 0.0f, 1.0f, 0.0f, height);
        float y = bounds.getBottom() - level;
        
        if (i == 0)
            p.startNewSubPath(x, y);
        else
            p.lineTo(x, y);
    }

    g.setColour(juce::Colours::lime);
    g.strokePath(p, juce::PathStrokeType(1.0f));
}

void AnalyzerComponent::drawMeter(juce::Graphics& g, const juce::Rectangle<int>& bounds, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(bounds);
    
    float meterHeight = bounds.getHeight() * level;
    auto meterBounds = bounds.withHeight(meterHeight).withY(bounds.getBottom() - meterHeight);
    
    g.setColour(level > 0.9f ? juce::Colours::red : juce::Colours::lime);
    g.fillRect(meterBounds);
}

void AnalyzerComponent::timerCallback()
{
    auto result = audioAnalyzer.getLastResult();
    
    smoothedRMS = smoothedRMS * SMOOTHING + result.rms * (1.0f - SMOOTHING);
    smoothedPeak = smoothedPeak * SMOOTHING + result.peak * (1.0f - SMOOTHING);
    
    repaint();
}

void AnalyzerComponent::resized()
{
    // Layout is handled in paint()
}