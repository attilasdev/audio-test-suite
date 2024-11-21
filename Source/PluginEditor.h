#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/SignalGeneratorComponent.h"
#include "Components/AnalyzerComponent.h"

class AudioTestSuiteEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioTestSuiteEditor(AudioTestSuiteProcessor&);
    ~AudioTestSuiteEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AudioTestSuiteProcessor& processorRef;
    
    SignalGeneratorComponent signalGeneratorComponent;
    AnalyzerComponent analyzerComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioTestSuiteEditor)
};