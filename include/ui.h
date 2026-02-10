#pragma once
#include "gamepad.h"
#include <string>
class UI
{
public:
    void draw(bool& showDemo, Gamepad* gamepad);
    void applyModernTheme();
private:
    void contentManager(Gamepad& gamepad);
    void drawContent(GamepadData& gamepadData); // Main Content
    void drawSearchContent(); // Searching Content

    std::string floatToString(float value);
};



