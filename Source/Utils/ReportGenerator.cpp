#include "ReportGenerator.h"

ReportGenerator::ReportGenerator()
{
}

void ReportGenerator::addTestResult(const TestReport& report)
{
    testReports.push_back(report);
}

void ReportGenerator::generateReport(const juce::File& outputFile)
{
    juce::String html = generateHTML();
    outputFile.replaceWithText(html);
}

void ReportGenerator::clearReports()
{
    testReports.clear();
}

juce::String ReportGenerator::generateHTML() const
{
    juce::String html;
    html << "<!DOCTYPE html><html><head><title>Audio Test Report</title></head><body>";
    html << "<h1>Audio Test Suite Report</h1>";
    
    for (const auto& report : testReports)
    {
        html << "<div class='test-case'>";
        html << "<h2>" << report.testName << "</h2>";
        html << "<p>Time: " << report.timestamp << "</p>";
        html << "<p>Status: " << (report.passed ? "PASSED" : "FAILED") << "</p>";
        html << "<h3>Results:</h3>";
        html << "<ul>";
        html << "<li>RMS Level: " << report.results.rms << "</li>";
        html << "<li>Peak Level: " << report.results.peak << "</li>";
        html << "<li>Fundamental Frequency: " << report.results.fundamentalFrequency << " Hz</li>";
        html << "</ul>";
        html << "<p>Notes: " << report.notes << "</p>";
        html << "</div>";
    }
    
    html << "</body></html>";
    return html;
}