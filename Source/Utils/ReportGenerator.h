#pragma once
#include <JuceHeader.h>
#include "../TestSuite/AudioAnalyzer.h"

class ReportGenerator
{
public:
    struct TestReport
    {
        juce::String testName;
        juce::String timestamp;
        AudioAnalyzer::AnalysisResult results;
        bool passed;
        juce::String notes;
    };
    
    ReportGenerator();
    
    void addTestResult(const TestReport& report);
    void generateReport(const juce::File& outputFile);
    void clearReports();
    
private:
    std::vector<TestReport> testReports;
    juce::String generateHTML() const;
};