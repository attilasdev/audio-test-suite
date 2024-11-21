#include "SignalGeneratorComponent.h"

SignalGeneratorComponent::SignalGeneratorComponent(SignalGenerator& generator)
    : signalGenerator(generator)
{
    // Waveform selector setup
    addAndMakeVisible(waveformSelector);
    waveformSelector.addItem("Sine", 1);
    waveformSelector.addItem("Square", 2);
    waveformSelector.addItem("White Noise", 3);
    waveformSelector.addItem("Off", 4);
    waveformSelector.onChange = [this] {
        auto type = static_cast<SignalGenerator::WaveType>(waveformSelector.getSelectedItemIndex());
        signalGenerator.setWaveType(type);
    };
    
    addAndMakeVisible(waveformLabel);
    waveformLabel.setText("Waveform", juce::dontSendNotification);
    waveformLabel.attachToComponent(&waveformSelector, true);
    
    // Frequency slider setup
    addAndMakeVisible(frequencySlider);
    frequencySlider.setRange(20.0, 20000.0, 1.0);
    frequencySlider.setSkewFactorFromMidPoint(1000.0);
    frequencySlider.setValue(440.0);
    frequencySlider.onValueChange = [this] {
        signalGenerator.setFrequency(static_cast<float>(frequencySlider.getValue()));
    };
    
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency (Hz)", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, true);
    
    // Amplitude slider setup
    addAndMakeVisible(amplitudeSlider);
    amplitudeSlider.setRange(0.0, 1.0, 0.01);
    amplitudeSlider.setValue(0.5);
    amplitudeSlider.onValueChange = [this] {
        signalGenerator.setAmplitude(static_cast<float>(amplitudeSlider.getValue()));
    };
    
    addAndMakeVisible(amplitudeLabel);
    amplitudeLabel.setText("Amplitude", juce::dontSendNotification);
    amplitudeLabel.attachToComponent(&amplitudeSlider, true);

    // Make sliders more responsive
    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    frequencySlider.setDoubleClickReturnValue(true, 440.0); // A4 note
    
    amplitudeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    amplitudeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    amplitudeSlider.setDoubleClickReturnValue(true, 0.5);
    
    amplitudeSlider.onValueChange = [this] {
        juce::MessageManager::callAsync([this] {
            signalGenerator.setAmplitude(static_cast<float>(amplitudeSlider.getValue()));
        });
    };
    
    waveformSelector.onChange = [this] {
        juce::MessageManager::callAsync([this] {
            auto type = static_cast<SignalGenerator::WaveType>(waveformSelector.getSelectedItemIndex());
            signalGenerator.setWaveType(type);
        });
    };
    // Use a timer for smoother frequency changes
    frequencySlider.onValueChange = [this] {
        if (!isTimerRunning())
            startTimer(50); // 50ms debounce
    };
    
    // Make amplitude changes immediate
    amplitudeSlider.onValueChange = [this] {
        signalGenerator.setAmplitude(static_cast<float>(amplitudeSlider.getValue()));
    };
    
    // Make waveform changes immediate
    waveformSelector.onChange = [this] {
        auto type = static_cast<SignalGenerator::WaveType>(waveformSelector.getSelectedItemIndex());
        signalGenerator.setWaveType(type);
    };
}

SignalGeneratorComponent::~SignalGeneratorComponent()
{
}

void SignalGeneratorComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SignalGeneratorComponent::resized()
{
    auto area = getLocalBounds().reduced(10);
    const int controlHeight = 24;
    const int labelWidth = 80;
    const int spacing = 20;
    
    // Layout controls with more space for the rotary frequency slider
    waveformSelector.setBounds(area.removeFromTop(controlHeight).withTrimmedLeft(labelWidth));
    area.removeFromTop(spacing);
    
    // Give more height to the frequency slider for the rotary control
    auto freqArea = area.removeFromTop(100);
    frequencySlider.setBounds(freqArea.withTrimmedLeft(labelWidth));
    area.removeFromTop(spacing);
    
    amplitudeSlider.setBounds(area.removeFromTop(controlHeight).withTrimmedLeft(labelWidth));
}
void SignalGeneratorComponent::timerCallback()
{
    signalGenerator.setFrequency(static_cast<float>(frequencySlider.getValue()));
    stopTimer();
}