#include "TestRunner.h"

TestRunner::TestRunner()
    : testBuffer(2, 4096)
{
}

void TestRunner::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    generator.prepare(spec);
    analyzer.prepare(spec);
}

void TestRunner::runFrequencyTest(float targetFreq)
{
    runTest("Frequency Test " + juce::String(targetFreq) + "Hz",
        [this, targetFreq]()
        {
            generator.setWaveType(SignalGenerator::WaveType::Sine);
            generator.setFrequency(targetFreq);
            generator.setAmplitude(1.0f);
        },
        [targetFreq](const AudioAnalyzer::AnalysisResult& results)
        {
            const float tolerance = 1.0f; // Hz
            return std::abs(results.fundamentalFrequency - targetFreq) < tolerance;
        });
}

void TestRunner::runAmplitudeTest(float targetAmplitude)
{
    runTest("Amplitude Test " + juce::String(targetAmplitude),
        [this, targetAmplitude]()
        {
            generator.setWaveType(SignalGenerator::WaveType::Sine);
            generator.setFrequency(1000.0f);
            generator.setAmplitude(targetAmplitude);
        },
        [targetAmplitude](const AudioAnalyzer::AnalysisResult& results)
        {
            const float tolerance = 0.01f;
            return std::abs(results.peak - targetAmplitude) < tolerance;
        });
}

void TestRunner::runTest(const juce::String& testName,
                        std::function<void()> setupFunc,
                        std::function<bool(const AudioAnalyzer::AnalysisResult&)> validateFunc)
{
    setupFunc();
    
    testBuffer.clear();
    generator.process(testBuffer);
    analyzer.analyze(testBuffer);
    
    auto results = analyzer.getLastResult();
    bool passed = validateFunc(results);
    
    ReportGenerator::TestReport report;
    report.testName = testName;
    report.timestamp = juce::Time::getCurrentTime().toString(true, true);
    report.results = results;
    report.passed = passed;
    report.notes = passed ? "Test passed successfully" : "Test failed";
    
    reportGenerator.addTestResult(report);
}