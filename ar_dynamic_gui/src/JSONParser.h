#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace ar_dynamic {

struct ControlSettings {
    juce::String id;
    juce::String name;
    float min, max, initialValue;
    int x, y, width, height;
};

struct ButtonSettings {
    juce::String id;
    juce::String name;
    juce::String type;
    int x, y, width, height;
    int default_state;
};


class JSONParser {
private:
    juce::String _jsonText;
    juce::var _json;
    juce::DynamicObject* _obj;
    std::vector<ControlSettings> _controls_settings;
    std::vector<ButtonSettings> _buttons_settings;
public:
    JSONParser(const juce::File& file)
    {
        if (file.existsAsFile()) {
             _jsonText = file.loadFileAsString();
            _json = juce::JSON::parse(_jsonText);
            _obj = _json.getDynamicObject();
        }
    }
    ~JSONParser() = default;
    bool parseLayoutFile() {
        if (_obj == nullptr) {
            return false;
        }
        juce::var controlsVar = _obj->getProperty("controls");
        if (auto* controlsArray = controlsVar.getArray()) {
            for (auto& c : *controlsArray) {
                ControlSettings s;
                s.id           = c.getProperty("id", "unknown").toString();
                s.name         = c.getProperty("name", "Parameter").toString();
                s.min          = c.getProperty("min", 0.0f);
                s.max          = c.getProperty("max", 1.0f);
                s.initialValue = c.getProperty("default", 0.5f);
                s.x            = c.getProperty("x", 0);
                s.y            = c.getProperty("y", 0);
                s.width        = c.getProperty("width", 80);
                s.height       = c.getProperty("height", 80);
                _controls_settings.push_back(s);
            }
        }

        controlsVar = _obj->getProperty("buttons");
        if (auto* controlsArray = controlsVar.getArray()) {
            for (auto& b : *controlsArray) {
                ButtonSettings s;
                s.id            = b.getProperty("id", "unknown").toString();
                s.name          = b.getProperty("name", "Parameter").toString();
                s.type          = b.getProperty("type", "unknown").toString();
                s.x             = b.getProperty("x", 0);
                s.y             = b.getProperty("y", 0);
                s.width         = b.getProperty("width", 80);
                s.height        = b.getProperty("height", 80);
                s.default_state = b.getProperty("default", 0);
                _buttons_settings.push_back(s);
            }
        }
        return true;
    }

    std::vector<ControlSettings> const& getControls() { return _controls_settings; }
    std::vector<ButtonSettings> const& getButtons() { return _buttons_settings; }
};

} // namespace ar_dynamic