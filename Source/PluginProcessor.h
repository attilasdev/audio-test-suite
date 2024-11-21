#pragma once

#include <JuceHeader.h>
#include "TestSuite/SignalGenerator.h"
#include "TestSuite/AudioAnalyzer.h"
#include "TestSuite/TestRunner.h"   


class AudioTestSuiteProcessor : public juce::AudioProcessor
{
public:

    SignalGenerator& getSignalGenerator() { return signalGenerator; }
    AudioAnalyzer& getAudioAnalyzer() { return audioAnalyzer; }

    AudioTestSuiteProcessor();
    ~AudioTestSuiteProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Audio Test Suite"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioTestSuiteProcessor)
    SignalGenerator signalGenerator;
    AudioAnalyzer audioAnalyzer;
    TestRunner testRunner;
    
    juce::dsp::ProcessSpec processSpec;
    juce::CriticalSection processLock;
};