/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <JuceHeader.h>
#include "Eko/Eko.h"
#include "EkoTime/EkoTime.h"
#include "EkoPreDelay/EkoPreDelay.h"

//==============================================================================
/**
*/
class EkoAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    EkoAudioProcessor();
    ~EkoAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

private:

    float mSizeSmooth;
    float mScaledFeedback;
    float mSpreadSmooth;
    float mCenterSmooth;
    float mDelayTimeSmooth;

    float mTimeArrayL[8] = { 0.0449278f, 0.127066f, 0.343338f, 0.510253f, 0.592813f, 0.709509f, 0.815368f, 0.934397f };
    float mTimeArrayR[8] = { 0.0327893f, 0.123349f, 0.233104f, 0.414528f, 0.494518f, 0.653244f, 0.747613f, 0.929841f };

    std::atomic<float>* mPreDelayTime;
    std::atomic<float>* mPreDelayFeedback;
    std::atomic<float>* mDiffusion;
    std::atomic<float>* mFeedback;
    std::atomic<float>* mSize;
    std::atomic<float>* mSpread;
    std::atomic<float>* mCenter;
    std::atomic<float>* mLP;
    std::atomic<float>* mHP;
    std::atomic<float>* mMix;
    std::atomic<float>* mColourCutoff;
    std::atomic<float>* mColourEmphasis;
    bool  mMute = false;

    Eko mEko;
    EkoTime mEkoTime;

    juce::AudioProcessorValueTreeState parameters;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EkoAudioProcessor)
};
