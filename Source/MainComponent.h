#pragma once

#include <JuceHeader.h>
#include "maximilian.h"
#include "Biquad.h"
#include "Biquad2.h"
#include "AudioFile/AudioFile.hpp"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void exportWavFile(juce::AudioBuffer<float> buffer, juce::File file);

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    

private:
    //==============================================================================
    
    float freqVal;
    float QVal;
    float ampVal = 1.f;
    
    float freqVal2;
    float QVal2 = 2.f;
    float ampVal2 = 0.1f;
    
    Biquad::FilterType filterType = Biquad::FilterType::HPF;
    Biquad2::FilterType2 filterType2 = Biquad2::FilterType2::LPF;
    
    maxiSample maxiSample, sample2;
    maxiFilter filter;
    Biquad biquad1;
    Biquad2 biquad2;
    
    maxiOsc testOsc;
    //member variables for saving file
    //providing full, absolute path seems to cause an error on my system.
    //juce::File file = juce::File("/users/USERNAME/JuceTestPlugins/SaveAudioFile/Source");
    juce::File file = juce::File::getCurrentWorkingDirectory().getChildFile ("Gramophone_Processed.wav");
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
