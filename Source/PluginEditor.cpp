/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EkoAudioProcessorEditor::EkoAudioProcessorEditor (EkoAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(mHighPassKnob);
    addAndMakeVisible(mLowPassKnob);
    addAndMakeVisible(mSizeKnob);
    addAndMakeVisible(mDiffusionKnob);
    addAndMakeVisible(mSpreadKnob);
    addAndMakeVisible(mCenterKnob);
    addAndMakeVisible(mMixKnob);
    addAndMakeVisible(mFeedbackKnob);
    addAndMakeVisible(mPreDelayTimeKnob);
    addAndMakeVisible(mPreDelayFeedbackKnob);
    addAndMakeVisible(mColourKnob);
    addAndMakeVisible(mEmphasisKnob);

    mHighPassKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "hp", mHighPassKnob));
    mLowPassKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lp", mLowPassKnob));
    mSizeKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "size", mSizeKnob));
    mDiffusionKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "diffusion", mDiffusionKnob));
    mCenterKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "center", mCenterKnob));
    mSpreadKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "spread", mSpreadKnob));
    mMixKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "mix", mMixKnob));
    mFeedbackKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "feedback", mFeedbackKnob));
    mPreDelayTimeKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "predelaytime", mPreDelayTimeKnob));
    mPreDelayFeedbackKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "predelayFeedback", mPreDelayFeedbackKnob));
    mColourKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "colourCutoff", mColourKnob));
    mEmphasisKnobAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "colourEmphasis", mEmphasisKnob));


    setSize (900, 600);
}

EkoAudioProcessorEditor::~EkoAudioProcessorEditor()
{
    
}

//==============================================================================
void EkoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    mBackground = juce::ImageCache::getFromMemory(BinaryData::EkoBackground_png, BinaryData::EkoBackground_pngSize);

    float backgroundWidth = (float)mBackground.getWidth();
    float backgroundHeight = (float)mBackground.getHeight();
    g.drawImageAt(mBackground, 0, 0);
}

void EkoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    mHighPassKnob.setScale(0.4);
    mHighPassKnob.setBounds(48, 454, 140, 140);
    mHighPassKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mHighPassKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    

    mLowPassKnob.setScale(0.4);
    mLowPassKnob.setBounds(148, 454, 140, 140);
    mLowPassKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mLowPassKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    

    mSizeKnob.setScale(0.7);
    mSizeKnob.setBounds(640, 196, 210, 210);
    mSizeKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSizeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    

    mFeedbackKnob.setScale(0.7);
    mFeedbackKnob.setBounds(345, 196, 210, 210);
    mFeedbackKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
   

    mDiffusionKnob.setScale(0.7);
    mDiffusionKnob.setBounds(50, 196, 210, 210);
    mDiffusionKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDiffusionKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
   

    mCenterKnob.setScale(0.5);
    mCenterKnob.setBounds(235, 312, 150, 150);
    mCenterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mCenterKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);


    mSpreadKnob.setScale(0.5);
    mSpreadKnob.setBounds(527, 312, 150, 150);
    mSpreadKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSpreadKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);


    mMixKnob.setScale(0.5);
    mMixKnob.setBounds(667, 30, 150, 150);
    mMixKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mMixKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    mPreDelayTimeKnob.setScale(0.5);
    mPreDelayTimeKnob.setBounds(77, 60, 150, 150);
    mPreDelayTimeKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPreDelayTimeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);


    mPreDelayFeedbackKnob.setScale(0.5);
    mPreDelayFeedbackKnob.setBounds(230, 120, 150, 150);
    mPreDelayFeedbackKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPreDelayFeedbackKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    mColourKnob.setScale(0.4);
    mColourKnob.setBounds(540, 454, 140, 140);
    mColourKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mColourKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);


    mEmphasisKnob.setScale(0.4);
    mEmphasisKnob.setBounds(680, 454, 140, 140);
    mEmphasisKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mEmphasisKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

}



