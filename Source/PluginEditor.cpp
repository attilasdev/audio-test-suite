#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioTestSuiteEditor::AudioTestSuiteEditor(AudioTestSuiteProcessor& p)
    : AudioProcessorEditor(&p)
    , processorRef(p)
    , signalGeneratorComponent(p.getSignalGenerator())
    , analyzerComponent(p.getAudioAnalyzer())
{
    addAndMakeVisible(signalGeneratorComponent);
    addAndMakeVisible(analyzerComponent);
    
    setSize(800, 400);
}

AudioTestSuiteEditor::~AudioTestSuiteEditor()
{
}

void AudioTestSuiteEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void AudioTestSuiteEditor::resized()
{
    auto bounds = getLocalBounds().reduced(4);
    signalGeneratorComponent.setBounds(bounds.removeFromLeft(300));
    analyzerComponent.setBounds(bounds);
}