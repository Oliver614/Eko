#pragma once


#include <JuceHeader.h>

//==============================================================================
/*
*/
class EkoLookAndFeel : public juce::LookAndFeel_V4
{
public:
    EkoLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    void drawLabel(juce::Graphics& g, juce::Label& label);

    void setScale(float scale) {mScale = scale;}


private:

    juce::Image knob;

    float mScale;
    float mKnobOption = 0;

};