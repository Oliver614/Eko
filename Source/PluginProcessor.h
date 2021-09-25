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

    void setFeedback(const float& feedback) { *mFeedback = feedback; };
    void setDiffusion(const float& diffusion) { *mDiffusion = diffusion; }
    void setSpread(const float& spread) { *mSpread = spread; }
    void setCenter(const float& center) { *mCenter = center; }
    void setMute(const float& mute) { *mMute = mute; }
    void setSize(const float& size) { *mSize = size; }
    void setMix(const float& mix) { *mMix = mix; }
    void setLP(const float& lp) { *mLP = lp; }
    void setHP(const float& hp) { *mHP = hp; }
    void setPreDelayTime(const float& delayTime) { *mPreDelayTime = delayTime; }
    void setPreDelayFeedback(const float& delayFeedback) { *mPreDelayFeedback = delayFeedback; }
    void setColourCutoff(const float& cutoff) { *mColourCutoff = cutoff; }
    void setColourEmphasis(const float& emphasis) { *mColourEmphasis = emphasis; }

    float getFeedback() { return *mFeedback; }
    float getDiffusion() { return *mDiffusion; }
    float getSpread() { return *mSpread; }
    float getCenter() { return *mCenter; }
    float getSize() { return *mSize; }
    float getMix() { return *mMix; }
    float getLP() { return *mLP; }
    float getHP() { return *mHP; }
    bool  getMute() { return *mMute; }

    float getPreDelayTime() { return *mPreDelayTime; }
    float getPreDelayFeedback() { return *mPreDelayFeedback; }

    float getColour() { return *mColourCutoff; }
    float getEmphasis() { return *mColourEmphasis; }

private:

    float mSizeSmooth;
    float mScaledFeedback;
    float mSpreadSmooth;
    float mCenterSmooth;
    float mDelayTimeSmooth;

    float mTimeArrayL[8] = { 0.0449278f, 0.127066f, 0.343338f, 0.510253f, 0.592813f, 0.709509f, 0.815368f, 0.934397f };
    float mTimeArrayR[8] = { 0.0327893f, 0.123349f, 0.233104f, 0.414528f, 0.494518f, 0.653244f, 0.747613f, 0.929841f };

    juce::AudioParameterFloat* mPreDelayTime;
    juce::AudioParameterFloat* mPreDelayFeedback;

    juce::AudioParameterFloat* mDiffusion;
    juce::AudioParameterFloat* mFeedback;
    juce::AudioParameterFloat* mSize;
    juce::AudioParameterFloat* mSpread;
    juce::AudioParameterFloat* mCenter;

    juce::AudioParameterFloat* mLP;
    juce::AudioParameterFloat* mHP;
    juce::AudioParameterFloat* mMix;
    juce::AudioParameterBool* mMute;

    juce::AudioParameterFloat* mColourCutoff;
    juce::AudioParameterFloat* mColourEmphasis;

    Eko mEko;
    EkoTime mEkoTime;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EkoAudioProcessor)
};
