/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./UI/EkoKnob.h"

//==============================================================================
/**
*/
class EkoAudioProcessorEditor  : public juce::AudioProcessorEditor
                                 
{
public:
    EkoAudioProcessorEditor (EkoAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~EkoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
   
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EkoAudioProcessor& audioProcessor;

    EkoKnob mHighPassKnob           ;
    EkoKnob mLowPassKnob            ;
    EkoKnob mSizeKnob               ;
    EkoKnob mDiffusionKnob          ;
    EkoKnob mCenterKnob             ;
    EkoKnob mSpreadKnob             ;
    EkoKnob mMixKnob                ;
    EkoKnob mFeedbackKnob           ;  
    EkoKnob mPreDelayTimeKnob       ;
    EkoKnob mPreDelayFeedbackKnob   ;
    EkoKnob mColourKnob             ;
    EkoKnob mEmphasisKnob           ;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mHighPassKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mLowPassKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSizeKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDiffusionKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mCenterKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSpreadKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mMixKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFeedbackKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPreDelayTimeKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPreDelayFeedbackKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mColourKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mEmphasisKnobAttachment;

    juce::Image mBackground;

    juce::AudioProcessorValueTreeState& valueTreeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EkoAudioProcessorEditor)
};
