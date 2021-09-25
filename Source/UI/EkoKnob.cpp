
#include "EkoKnob.h"

EkoKnob::EkoKnob() : Slider()
                                        
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    
    setLookAndFeel(&SliderLookAndFeel);
}

EkoKnob::~EkoKnob()
{
    setLookAndFeel(nullptr);
}

void EkoKnob::mouseDown(const juce::MouseEvent& event)
{
    juce::Slider::mouseDown(event);
    setMouseCursor(juce::MouseCursor::NoCursor);
    mousePosition = juce::Desktop::getMousePosition();
}

void EkoKnob::mouseUp(const juce::MouseEvent& event)
{
    Slider::mouseUp(event);
    juce::Desktop::setMousePosition(mousePosition);
    setMouseCursor(juce::MouseCursor::NormalCursor);
}