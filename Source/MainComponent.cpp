#include "MainComponent.h"
#include "AudioFile/AudioFile.hpp"
#include <iostream>

//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 600);

    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    file.deleteFile();
    //if file fails to load, provide full path for file or change working directory if possible
    maxiSample.load("crackle.wav");
    sample2.load("gramophoneTest.wav");
    
    biquad1.setFs(sampleRate);
    biquad2.setFs(sampleRate);
    
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::AudioBuffer<float> buffer(2, 288000);
    float* leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    float* rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    auto leftSpeaker1 = buffer.getWritePointer(0);
    auto rightSpeaker1 = buffer.getWritePointer(1);
    
    
    //setting values for filter cutoff frequencies
    freqVal = 6000;
    freqVal2 = 1300;
    QVal = 1.0f;
    ampVal = 1.f;

    //assigning values
    biquad1.setFreq(freqVal);
    biquad1.setQ(QVal);
    biquad1.setAmpdB(ampVal);
    biquad1.setFilterType(filterType);

    biquad2.setFreq(freqVal2);
    biquad2.setQ(QVal);
    biquad2.setAmpdB(ampVal2);
    biquad2.setFilterType(filterType2);
    
    for(int sample = 0; sample < buffer.getNumSamples(); ++sample){
        float w = maxiSample.play4(0.1, 1.0, maxiSample.getLength());
        float x = sample2.play4(0.18, 1.0, sample2.getLength());

        float y = biquad2.processSample(x, 1);
        float z = biquad1.processSample(y, 1);
        leftSpeaker1[sample] = (1.3 * z) + w;
        rightSpeaker1[sample] = leftSpeaker1[sample];
    }
    juce::WavAudioFormat format;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    writer.reset(format.createWriterFor(new juce::FileOutputStream(file), 44100.0, 1, 16, {}, 0));
    writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}



void MainComponent::releaseResources()
{
    
}


//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
