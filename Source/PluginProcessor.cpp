#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioTestSuiteProcessor::AudioTestSuiteProcessor()
     : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
        // Initialize with default values
    processSpec.sampleRate = 44100.0;
    processSpec.maximumBlockSize = 512;
    processSpec.numChannels = 2;
    
    // Prepare components with default specs
    signalGenerator.prepare(processSpec);
    audioAnalyzer.prepare(processSpec);
}

AudioTestSuiteProcessor::~AudioTestSuiteProcessor()
{
}

void AudioTestSuiteProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Request realtime priority
    juce::Process::setPriority(juce::Process::HighPriority);
    
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = static_cast<uint32>(samplesPerBlock);
    processSpec.numChannels = static_cast<uint32>(getTotalNumOutputChannels());
    
    signalGenerator.prepare(processSpec);
    audioAnalyzer.prepare(processSpec);
    testRunner.prepare(processSpec);
}

void AudioTestSuiteProcessor::releaseResources()
{
    // Free resources when playback stops
}

void AudioTestSuiteProcessor::processBlock (juce::AudioBuffer<float>& buffer, 
                                          juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    const juce::ScopedLock sl(processLock);  // Add this
    
    buffer.clear();
    signalGenerator.process(buffer);
    audioAnalyzer.analyze(buffer);
}

void AudioTestSuiteProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Store plugin state here
}

void AudioTestSuiteProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore plugin state here
}

juce::AudioProcessorEditor* AudioTestSuiteProcessor::createEditor()
{
    return new AudioTestSuiteEditor(*this);
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioTestSuiteProcessor();
}