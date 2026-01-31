#pragma once
#include <JuceHeader.h>

class PedalComponent : public juce::Component
{
public:
    PedalComponent(juce::AudioProcessorValueTreeState& vts, int numberOfKnobs)
    {
        addAndMakeVisible(bypassButton);
        bypassButton.setButtonText("BYPASS");
        buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(vts, "bypass", bypassButton);

        for (int i = 0; i < numberOfKnobs; ++i)
        {
            auto* slider = new juce::Slider();
            slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            
            addAndMakeVisible(slider);
            knobs.add(slider);

            knobAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                vts, "knob" + juce::String(i), *slider));
        }
    }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        g.setColour(juce::Colours::darkred); // צבע פדאל קלאסי
        g.fillRoundedRectangle(bounds, 10.0f);
        
        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(bounds.reduced(2.0f), 10.0f, 2.0f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(15);
        
        bypassButton.setBounds(area.removeFromBottom(40).withSizeKeepingCentre(80, 30));
        
        if (knobs.size() > 0)
        {
            int knobWidth = area.getWidth() / knobs.size();
            for (auto* knob : knobs)
            {
                knob->setBounds(area.removeFromLeft(knobWidth).reduced(5));
            }
        }
    }

private:
    juce::TextButton bypassButton;
    juce::OwnedArray<juce::Slider> knobs;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> knobAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PedalComponent)
};