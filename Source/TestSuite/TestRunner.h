#pragma once
#include <JuceHeader.h>
#include "SignalGenerator.h"
#include "AudioAnalyzer.h"
#include "../Utils/ReportGenerator.h"

class TestRunner
{
public:
    TestRunner();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void runFrequencyTest(float targetFreq);
    void runAmplitudeTest(float targetAmplitude);
    void runNoiseTest();
    
    const ReportGenerator& getReportGenerator() const { return reportGenerator; }
    
private:
    SignalGenerator generator;
    AudioAnalyzer analyzer;
    ReportGenerator reportGenerator;
    
    juce::AudioBuffer<float> testBuffer;
    double currentSampleRate = 44100.0;
    
    void runTest(const juce::String& testName, 
                 std::function<void()> setupFunc,
                 std::function<bool(const AudioAnalyzer::AnalysisResult&)> validateFunc);
};