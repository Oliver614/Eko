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
class EkoAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                 public juce::Slider::Listener
{
public:
    EkoAudioProcessorEditor (EkoAudioProcessor&);
    ~EkoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* sliderThatWasMoved) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EkoAudioProcessor& audioProcessor;

    EkoKnob* mHighPassKnob;
    EkoKnob* mLowPassKnob;
    EkoKnob* mSizeKnob;
    EkoKnob* mDiffusionKnob;
    EkoKnob* mCenterKnob;
    EkoKnob* mSpreadKnob;
    EkoKnob* mMixKnob;
    EkoKnob* mFeedbackKnob;
    EkoKnob* mPreDelayTimeKnob;
    EkoKnob* mPreDelayFeedbackKnob;
    EkoKnob* mColourKnob;
    EkoKnob* mEmphasisKnob;


    juce::Image mBackground;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EkoAudioProcessorEditor)
};
