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

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

class JSONParser {
public:
    JSONParser(const juce::File& file) {
        if (file.existsAsFile()) {
            _jsonText = file.loadFileAsString();
            _json = juce::JSON::parse(_jsonText);
            _obj = _json.getDynamicObject();
        }
    }

    bool parseLayoutFile() {
        if (_obj == nullptr) return false;
        _all_settings.clear();

        auto parseArray = [this](const juce::String& propertyName, auto parseFunc) {
            juce::var data = _obj->getProperty(propertyName);
            if (auto* array = data.getArray()) {
                for (auto& item : *array) {
                    _all_settings.push_back(parseFunc(item));
                }
            }
            };

        parseArray("controls", [](const juce::var& c) -> AnyControlSettings {
            ControlSettings s;
            s.id            = c.getProperty("id", "unknown").toString();
            s.name          = c.getProperty("name", "Parameter").toString();
            s.min           = c.getProperty("min", 0.0f);
            s.max           = c.getProperty("max", 1.0f);
            s.initialValue  = c.getProperty("default", 0.5f);
            s.x             = c.getProperty("x", 0); 
            s.y             = c.getProperty("y", 0);
            s.width         = c.getProperty("width", 80); 
            s.height        = c.getProperty("height", 80);
            return s;
            });

        parseArray("buttons", [](const juce::var& b) -> AnyControlSettings {
            ButtonSettings s;
            s.id            = b.getProperty("id", "unknown").toString();
            s.name          = b.getProperty("name", "Parameter").toString();
            s.type          = b.getProperty("type", "unknown").toString();
            s.x             = b.getProperty("x", 0); 
            s.y             = b.getProperty("y", 0);
            s.width         = b.getProperty("width", 80); 
            s.height        = b.getProperty("height", 80);
            s.default_state = b.getProperty("default", 0);
            return s;
            });

        return true;
    }

    template <typename ControlFunc, typename ButtonFunc>
    void process(ControlFunc&& onControl, ButtonFunc&& onButton) const {
        for (const auto& config : _all_settings) {
            std::visit(overloaded{
                std::forward<ControlFunc>(onControl),
                std::forward<ButtonFunc>(onButton)
                }, config);
        }
    }

private:
    using AnyControlSettings = std::variant<ControlSettings, ButtonSettings>;

    juce::String _jsonText;
    juce::var _json;
    juce::DynamicObject* _obj;
    std::vector<AnyControlSettings> _all_settings;
};

} // namespace ar_dynamic