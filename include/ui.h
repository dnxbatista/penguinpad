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
    void drawContent(Gamepad& gamepad); // Main Content
    void drawSearchContent(); // Searching Content

    std::string floatToString(float value);

    float m_lightbarColor[3] = { 0.0f, 0.5f, 1.0f };
    bool m_showLightbarModal = false;
};



