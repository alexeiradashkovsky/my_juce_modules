#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace ar_dynamic {

class CustomKnobLookAndFeel : public juce::LookAndFeel_V4 {
public:
    CustomKnobLookAndFeel(juce::Image knobImage) : _imageStrip(knobImage) {
        _numFrames = _imageStrip.getHeight() / _imageStrip.getWidth();
    }
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                      float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                      juce::Slider& slider) override 
    {
        auto radius = juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = x + width * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(juce::Colour(0xFF1B3022));
        g.fillEllipse(rx, ry, rw, rw);

        g.setColour(juce::Colours::silver.withAlpha(0.5f));
        g.drawEllipse(rx, ry, rw, rw, 2.0f);

        juce::Path p;
        auto pointerLength = radius * 0.8f;
        auto pointerThickness = 3.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(juce::Colours::white);
        g.fillPath(p);
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
        auto centerX = bounds.getCentreX();
        auto centerY = bounds.getCentreY();

        juce::ColourGradient silverGrad(juce::Colours::grey, centerX, centerY - (size * 0.3f),
            juce::Colours::darkgrey, centerX, centerY + (size * 0.3f), false);
        g.setGradientFill(silverGrad);
        g.fillEllipse(centerX - (size * 0.4f), centerY - (size * 0.4f), size * 0.8f, size * 0.8f);

        g.setColour(juce::Colours::black.withAlpha(0.4f));
        g.drawEllipse(centerX - (size * 0.3f), centerY - (size * 0.3f), size * 0.6f, size * 0.6f, 1.0f);

        // LED
        bool isOn = button.getToggleState();
        float ledSize = size * 0.1f;
        float ledY = centerY - (size * 0.6f);

        if (isOn) {
            g.setColour(juce::Colours::red);
            g.fillEllipse(centerX - (ledSize * 0.5f), ledY, ledSize, ledSize);
            g.setColour(juce::Colours::red.withAlpha(0.3f));
            g.fillEllipse(centerX - ledSize, ledY - (ledSize * 0.5f), ledSize * 2, ledSize * 2);
        }
        else {
            g.setColour(juce::Colours::darkred.withAlpha(0.2f));
            g.fillEllipse(centerX - (ledSize * 0.5f), ledY, ledSize, ledSize);
        }
    }
private:
    juce::Image _imageStrip;
    int _numFrames;
};

} // namespace ar_dynamic