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
        for (auto const& config : _parser.getControls())
        {
            auto* newSlider = new juce::Slider();
            newSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            newSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            newSlider->setLookAndFeel(_customLookAndFeel.get());

            _sliders.add(newSlider);
            addAndMakeVisible(newSlider);
            
            _attachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(
                _apvts, config.id, *newSlider));

            auto* newLabel = new juce::Label(config.id, config.name);
            newLabel->setJustificationType(juce::Justification::centred);
            newLabel->setFont(juce::Font(14.0f, juce::Font::bold));

            _labels.add(newLabel);
            addAndMakeVisible(newLabel);
        }

        for (auto const& config : _parser.getButtons())
        {
            auto* newButton = new juce::ToggleButton(config.id);
            newButton->setLookAndFeel(_customLookAndFeel.get());
            newButton->setBounds(config.x, config.y, config.width, config.height);
            _toggle_buttons.add(newButton);
            addAndMakeVisible(newButton);
            _buttonAttachments.add(new juce::AudioProcessorValueTreeState::ButtonAttachment(
                _apvts, config.id, *newButton));
        }

        setSize(800, 900);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colour(0xFF222222));
    }

    void resized() override {
        for (int i = 0; i < _sliders.size(); ++i)
        {
            auto const& config = _parser.getControls()[i];
            _sliders[i]->setBounds(config.x, config.y, config.width, config.height);
            _labels[i]->setBounds(config.x, config.y - 25, config.width, 20);
        }

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