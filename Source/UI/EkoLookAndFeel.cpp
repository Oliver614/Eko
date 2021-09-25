#include <JuceHeader.h>
#include "EkoLookAndFeel.h"

//==============================================================================
EkoLookAndFeel::EkoLookAndFeel() : juce::LookAndFeel_V4()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    mScale= 0.f;
    knob = juce::ImageCache::getFromMemory(BinaryData::EkoKnob_png, BinaryData::EkoKnob_pngSize);
    

}

void EkoLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    if (knob.isValid())
    {
        const double rotation = (slider.getValue()
            - slider.getMinimum())
            / (slider.getMaximum()
                - slider.getMinimum());

        int frames = 270;
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = juce::jmin(width / 1.0f, height / 1.0f);
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.f;
        const float ry = centerY - radius - 1.f;

        int imgWidth = knob.getWidth();
        int imgHeight = knob.getHeight() / frames;
        g.drawImage(knob, 0, 0, imgWidth*mScale, imgHeight*mScale, 0, frameId * imgHeight, imgWidth, imgHeight);
    }
    else
    {
        static const float textPpercent = 0.35f;
        juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);

        g.setColour(juce::Colours::white);

        g.drawFittedText(juce::String("No Image"), text_bounds.getSmallestIntegerContainer(), juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
    }
}

void EkoLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(juce::Colour(juce::uint8(255), 255, 255, 1.0f));
    g.fillRoundedRectangle(label.getLocalBounds().toFloat(), 3.0f);


    if (!label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font(getLabelFont(label));

        g.setColour(juce::Colour(juce::uint8(0), 0, 0, 1.0f));
        g.setFont(font);

        auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
            juce::jmax(1, (int)(textArea.getHeight() / font.getHeight())),
            label.getMinimumHorizontalScale());

        g.setColour(juce::Colour(juce::uint8(255), 255, 255, 0.1f));
    }
    else if (label.isEnabled())
    {
        g.setColour(label.findColour(juce::Label::outlineColourId));
    }

    //g.fillRoundedRectangle(label.getLocalBounds().toFloat(), 3.0f);
}
