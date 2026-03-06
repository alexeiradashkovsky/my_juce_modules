#include "CustomKnobLookAndFeel.h"

namespace ar_dynamic {

class DynamicEditor : public juce::AudioProcessorEditor 
{
public:
    DynamicEditor(juce::AudioProcessor& p, juce::AudioProcessorValueTreeState& vts, juce::File jsonFile, juce::File knobSkin)
        : AudioProcessorEditor(&p), _parser{ jsonFile }, _apvts(vts)
    {
        
        juce::Image knobImg = juce::ImageFileFormat::loadFrom(jsonFile.getSiblingFile("knob_skin.png"));
        if (knobImg.isValid()) {
            _customLookAndFeel = std::make_unique<CustomKnobLookAndFeel>(knobImg);
        }
        _customLookAndFeel = std::make_unique<CustomKnobLookAndFeel>(knobImg);

        _parser.parseLayoutFile();
        _parser.process(
            [this](const ControlSettings& s) {
                auto* newSlider = _sliders.add(std::make_unique<juce::Slider>());
                newSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
                newSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
                newSlider->setLookAndFeel(_customLookAndFeel.get());
                addAndMakeVisible(newSlider);

                _attachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                    _apvts, s.id, *newSlider));

                auto* newLabel = _labels.add(std::make_unique<juce::Label>(s.id, s.name));
                newLabel->setJustificationType(juce::Justification::centred);
                newLabel->setFont(juce::Font(14.0f, juce::Font::bold));
                addAndMakeVisible(newLabel);
            },
            [this](const ButtonSettings& s) {
                auto* newButton = _toggle_buttons.add(std::make_unique<juce::ToggleButton>(s.id));
                newButton->setLookAndFeel(_customLookAndFeel.get());
                addAndMakeVisible(newButton);

                _buttonAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
                    _apvts, s.id, *newButton));
            }
        );

        setSize(800, 900);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colour(0xFF222222));
    }

    void resized() override {
        int sliderIdx = 0;
        int buttonIdx = 0;

        _parser.process(
            [&](const ControlSettings& s) {
                if (sliderIdx < _sliders.size()) {
                    _sliders[sliderIdx]->setBounds(s.x, s.y, s.width, s.height);
                    _labels[sliderIdx]->setBounds(s.x, s.y - 25, s.width, 20);
                    sliderIdx++;
                }
            },
            [&](const ButtonSettings& s) {
                if (buttonIdx < _toggle_buttons.size()) {
                    _toggle_buttons[buttonIdx]->setBounds(s.x, s.y, s.width, s.height);
                    buttonIdx++;
                }
            }
        );
    }

private:
    JSONParser _parser;
    std::unique_ptr<CustomKnobLookAndFeel> _customLookAndFeel;
    juce::AudioProcessorValueTreeState& _apvts;

    juce::OwnedArray<juce::Slider> _sliders;
    juce::OwnedArray<juce::Label> _labels;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> _attachments;
    juce::OwnedArray<juce::ToggleButton> _toggle_buttons;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> _buttonAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DynamicEditor)
};
} // namespace ar_dynamic