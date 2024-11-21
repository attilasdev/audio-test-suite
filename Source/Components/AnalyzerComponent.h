#pragma once
#include <JuceHeader.h>
#include "../TestSuite/AudioAnalyzer.h"

class AnalyzerComponent : public juce::Component,
                         private juce::Timer
{
public:
    AnalyzerComponent(AudioAnalyzer& analyzer);
    ~AnalyzerComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void drawSpectrum(juce::Graphics& g, const juce::Rectangle<int>& bounds);
    void drawMeter(juce::Graphics& g, const juce::Rectangle<int>& bounds, float level);

    AudioAnalyzer& audioAnalyzer;
    float smoothedRMS = 0.0f;
    float smoothedPeak = 0.0f;
    
    static constexpr int TIMER_HZ = 15;
    static constexpr float SMOOTHING = 0.9f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyzerComponent)
};