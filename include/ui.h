#pragma once
#include "controller.h"
class UI
{
    public:
        void draw(bool& showDemo, Controller* controller);
    private:
        void contentManager(Controller& controller);
        void drawContent(GamepadData& gamepadData); // Main Content
        void drawSearchContent(); // Searching Content

};



