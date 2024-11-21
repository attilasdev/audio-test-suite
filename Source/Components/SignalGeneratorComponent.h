#pragma once
#include <JuceHeader.h>
#include "../TestSuite/SignalGenerator.h"

class SignalGeneratorComponent : public juce::Component,
                                 private juce::Timer
{
public:
    SignalGeneratorComponent(SignalGenerator& generator);
    ~SignalGeneratorComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override; 
    SignalGenerator& signalGenerator;
    
    juce::ComboBox waveformSelector;
    juce::Slider frequencySlider;
    juce::Slider amplitudeSlider;
    
    juce::Label waveformLabel;
    juce::Label frequencyLabel;
    juce::Label amplitudeLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SignalGeneratorComponent)
};