#pragma once


#pragma once

#include <JuceHeader.h>
#include "EkoLookAndFeel.h"

//==============================================================================
/*
*/
class EkoKnob : public juce::Slider
{
public:
    EkoKnob();
    ~EkoKnob();

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void setScale(float scale) {SliderLookAndFeel.setScale(scale);}


private:
    
    EkoLookAndFeel SliderLookAndFeel;
    juce::Point<int> mousePosition;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EkoKnob)
};