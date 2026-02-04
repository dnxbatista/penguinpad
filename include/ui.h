#pragma once
#include "gamepad.h"
class UI
{
    public:
        void draw(bool& showDemo, Gamepad* gamepad);
    private:
        void contentManager(Gamepad& gamepad);
        void drawContent(GamepadData& gamepadData); // Main Content
        void drawSearchContent(); // Searching Content

};



