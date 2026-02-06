/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION

  ID:               ar_dynamic_gui
  vendor:           AlexeiRadashkovsky
  version:          1.0.0
  name:             AR Dynamic GUI
  description:      JSON-driven dynamic UI for guitar effects.
  dependencies:     juce_gui_basics, juce_audio_processors, juce_data_structures
  website:          https://github.com/alexeiradashkovsky/my_juce_modules
  license:          MIT

 END_JUCE_MODULE_DECLARATION
*******************************************************************************/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "src/JSONParser.h"
#include "src/DynamicEditor.h"
// CustomKnob.h