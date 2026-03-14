# AR Dynamic GUI

## Description

AR Dynamic GUI is a JUCE module that allows you to create dynamic user interfaces for audio plugins and applications based on JSON configuration files. This enables you to define the layout, parameters, and properties of UI elements without recompiling the code.

## Features

*   **JSON-Driven UI:** Define UI layouts and parameters using JSON files.
*   **Dynamic Updates:** Modify the UI by simply changing the JSON configuration.
*   **Customizable Components:** Supports sliders, buttons, and other JUCE components.
*   **Easy Integration:** Easily integrate into existing JUCE projects.

## Dependencies

*   juce\_gui\_basics
*   juce\_audio\_processors
*   juce\_data\_structures

## Installation

1.  Clone this repository into your JUCE modules directory.
2.  Add the module to your Projucer project.
3.  Include the module header in your project:

    ```cpp
    #include <ar_dynamic_gui/ar_dynamic_gui.h>
    ```

## Usage

1.  Create a JSON file that defines the layout and parameters of your UI.
2.  Use the `JSONParser` class to parse the JSON file and create the UI components.
3.  Add the components to your audio processor editor.
