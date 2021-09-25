/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EkoAudioProcessorEditor::EkoAudioProcessorEditor (EkoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(mHighPassKnob = new EkoKnob);
    addAndMakeVisible(mLowPassKnob = new EkoKnob);
    addAndMakeVisible(mSizeKnob = new EkoKnob);
    addAndMakeVisible(mDiffusionKnob = new EkoKnob);
    addAndMakeVisible(mSpreadKnob = new EkoKnob);
    addAndMakeVisible(mCenterKnob = new EkoKnob);
    addAndMakeVisible(mMixKnob = new EkoKnob);
    addAndMakeVisible(mFeedbackKnob = new EkoKnob);
    addAndMakeVisible(mPreDelayTimeKnob = new EkoKnob);
    addAndMakeVisible(mPreDelayFeedbackKnob = new EkoKnob);
    addAndMakeVisible(mColourKnob = new EkoKnob);
    addAndMakeVisible(mEmphasisKnob = new EkoKnob);


    setSize (900, 600);
}

EkoAudioProcessorEditor::~EkoAudioProcessorEditor()
{
    delete mHighPassKnob;
    delete mLowPassKnob;
    delete mSizeKnob;
    delete mDiffusionKnob;
    delete mSpreadKnob;
    delete mCenterKnob;
    delete mMixKnob;
    delete mFeedbackKnob;
    delete mPreDelayFeedbackKnob;
    delete mPreDelayTimeKnob;
    delete mEmphasisKnob;
    delete mColourKnob;
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

    mHighPassKnob->setScale(0.4);
    mHighPassKnob->setBounds(48, 454, 140, 140);
    mHighPassKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mHighPassKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mHighPassKnob->setRange(0.0, 1.0);
    mHighPassKnob->setValue(audioProcessor.getHP());
    mHighPassKnob->addListener(this);

    mLowPassKnob->setScale(0.4);
    mLowPassKnob->setBounds(148, 454, 140, 140);
    mLowPassKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mLowPassKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mLowPassKnob->setRange(0.0, 1.0);
    mLowPassKnob->setValue(audioProcessor.getLP());
    mLowPassKnob->addListener(this);

    mSizeKnob->setScale(0.7);
    mSizeKnob->setBounds(640, 196, 210, 210);
    mSizeKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSizeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mSizeKnob->setRange(0.0, 1.0);
    mSizeKnob->setValue(audioProcessor.getSize());
    mSizeKnob->addListener(this);

    mFeedbackKnob->setScale(0.7);
    mFeedbackKnob->setBounds(345, 196, 210, 210);
    mFeedbackKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mFeedbackKnob->setRange(0.0, 1.0);
    mFeedbackKnob->setValue(audioProcessor.getFeedback());
    mFeedbackKnob->addListener(this);

    mDiffusionKnob->setScale(0.7);
    mDiffusionKnob->setBounds(50, 196, 210, 210);
    mDiffusionKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDiffusionKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mDiffusionKnob->setRange(0.0, 1.0);
    mDiffusionKnob->setValue(audioProcessor.getDiffusion());
    mDiffusionKnob->addListener(this);

    mCenterKnob->setScale(0.5);
    mCenterKnob->setBounds(235, 312, 150, 150);
    mCenterKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mCenterKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mCenterKnob->setRange(0.0, 1.0);
    mCenterKnob->setValue(audioProcessor.getCenter());
    mCenterKnob->addListener(this);

    mSpreadKnob->setScale(0.5);
    mSpreadKnob->setBounds(527, 312, 150, 150);
    mSpreadKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSpreadKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mSpreadKnob->setRange(0.0, 1.0);
    mSpreadKnob->setValue(audioProcessor.getSpread());
    mSpreadKnob->addListener(this);

    mMixKnob->setScale(0.5);
    mMixKnob->setBounds(667, 30, 150, 150);
    mMixKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mMixKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mMixKnob->setRange(0.0, 1.0);
    mMixKnob->setValue(audioProcessor.getMix());
    mMixKnob->addListener(this);

    mPreDelayTimeKnob->setScale(0.5);
    mPreDelayTimeKnob->setBounds(77, 60, 150, 150);
    mPreDelayTimeKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPreDelayTimeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mPreDelayTimeKnob->setRange(0.0, 2.0);
    mPreDelayTimeKnob->setValue(audioProcessor.getPreDelayTime());
    mPreDelayTimeKnob->addListener(this);

    mPreDelayFeedbackKnob->setScale(0.5);
    mPreDelayFeedbackKnob->setBounds(230, 120, 150, 150);
    mPreDelayFeedbackKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPreDelayFeedbackKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mPreDelayFeedbackKnob->setRange(0.0, 1.0);
    mPreDelayFeedbackKnob->setValue(audioProcessor.getPreDelayFeedback());
    mPreDelayFeedbackKnob->addListener(this);

    mColourKnob->setScale(0.4);
    mColourKnob->setBounds(540, 454, 140, 140);
    mColourKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mColourKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mColourKnob->setRange(-1.0, 1.0);
    mColourKnob->setValue(audioProcessor.getColour());
    mColourKnob->addListener(this);

    mEmphasisKnob->setScale(0.4);
    mEmphasisKnob->setBounds(680, 454, 140, 140);
    mEmphasisKnob->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mEmphasisKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mEmphasisKnob->setRange(0.0, 1.0);
    mEmphasisKnob->setValue(audioProcessor.getColour());
    mEmphasisKnob->addListener(this);

}

void EkoAudioProcessorEditor::sliderValueChanged(juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == mFeedbackKnob)
    {
        audioProcessor.setFeedback(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mMixKnob)
    {
        audioProcessor.setMix(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mCenterKnob)
    {
        audioProcessor.setCenter(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mSpreadKnob)
    {
        audioProcessor.setSpread(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mDiffusionKnob)
    {
        audioProcessor.setDiffusion(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mSizeKnob)
    {
        audioProcessor.setSize(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mHighPassKnob)
    {
        audioProcessor.setHP(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mLowPassKnob)
    {
        audioProcessor.setLP(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mPreDelayTimeKnob)
    {
        audioProcessor.setPreDelayTime(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mPreDelayFeedbackKnob)
    {
        audioProcessor.setPreDelayFeedback(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mColourKnob)
    {
        audioProcessor.setColourCutoff(sliderThatWasMoved->getValue());
    }
    if (sliderThatWasMoved == mEmphasisKnob)
    {
        audioProcessor.setColourEmphasis(sliderThatWasMoved->getValue());
    }

}
