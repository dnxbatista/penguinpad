#include "ui.h"
#include "gamepad.h"
#include <imgui.h>
#include <cmath>
#include <string>
#include <sstream>
#include <format>
#include <cstdint>

std::string getGamepadType(SDL_GamepadType gamepadType);
std::string floatToString(float value);

void UI::draw(bool& showDemo, Gamepad* gamepad)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    window_flags |= ImGuiWindowFlags_MenuBar;

	ImGui::Begin("penguinpad", nullptr, window_flags);
    // Draw --------------
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Gamepad Light"))
            {
                m_showLightbarModal = true;
            }
            if (ImGui::MenuItem("Gamepad Vibration"))
            {
                m_showVibrationModal = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (m_showLightbarModal)
    {
        ImGui::OpenPopup("Gamepad Light");
        if (ImGui::BeginPopupModal("Gamepad Light", &m_showLightbarModal, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Lightbar");
            ImGui::ColorEdit3("Color", m_lightbarColor);
            if (ImGui::Button("Update"))
            {
                uint8_t r = static_cast<uint8_t>(m_lightbarColor[0] * 255.0f);
                uint8_t g = static_cast<uint8_t>(m_lightbarColor[1] * 255.0f);
                uint8_t b = static_cast<uint8_t>(m_lightbarColor[2] * 255.0f);
                gamepad->setLightbar(r, g, b);
            }
            ImGui::SameLine();
            if (ImGui::Button("Close"))
            {
                m_showLightbarModal = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    if (m_showVibrationModal)
    {
        ImGui::OpenPopup("Gamepad Vibration");
        if (ImGui::BeginPopupModal("Gamepad Vibration", &m_showVibrationModal, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Vibration Tester");
            ImGui::Separator();

            ImGui::Text("Presets");
            if (ImGui::Button("Light"))
            {
                m_rumbleLow = 0.25f;
                m_rumbleHigh = 0.35f;
                m_triggerLeft = 0.25f;
                m_triggerRight = 0.25f;
            }
            ImGui::SameLine();
            if (ImGui::Button("Medium"))
            {
                m_rumbleLow = 0.5f;
                m_rumbleHigh = 0.65f;
                m_triggerLeft = 0.5f;
                m_triggerRight = 0.5f;
            }
            ImGui::SameLine();
            if (ImGui::Button("Strong"))
            {
                m_rumbleLow = 0.8f;
                m_rumbleHigh = 1.0f;
                m_triggerLeft = 0.8f;
                m_triggerRight = 0.8f;
            }

            ImGui::Spacing();
            ImGui::Text("Duration (ms)");
            ImGui::SliderInt("##RumbleDuration", &m_rumbleDurationMs, 50, 2000);

            ImGui::Spacing();
            ImGui::Text("Rumble (Low / High)");
            ImGui::SliderFloat("Low", &m_rumbleLow, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat("High", &m_rumbleHigh, 0.0f, 1.0f, "%.2f");
            if (ImGui::Button("Test Rumble"))
            {
                bool ok = gamepad->rumble(m_rumbleLow, m_rumbleHigh, static_cast<uint32_t>(m_rumbleDurationMs));
                m_rumbleStatus = ok ? "Rumble sent" : "Rumble not supported";
            }
            if (!m_rumbleStatus.empty())
            {
                ImGui::SameLine();
                ImGui::TextDisabled("%s", m_rumbleStatus.c_str());
            }

            ImGui::Spacing();
            ImGui::Text("Triggers (Left / Right)");
            ImGui::SliderFloat("Left", &m_triggerLeft, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat("Right", &m_triggerRight, 0.0f, 1.0f, "%.2f");
            if (ImGui::Button("Test Triggers"))
            {
                bool ok = gamepad->rumbleTriggers(m_triggerLeft, m_triggerRight, static_cast<uint32_t>(m_rumbleDurationMs));
                m_triggerStatus = ok ? "Trigger rumble sent" : "Trigger rumble not supported";
            }
            if (!m_triggerStatus.empty())
            {
                ImGui::SameLine();
                ImGui::TextDisabled("%s", m_triggerStatus.c_str());
            }

            ImGui::Spacing();
            if (ImGui::Button("Stop All"))
            {
                gamepad->rumble(0.0f, 0.0f, 0);
                gamepad->rumbleTriggers(0.0f, 0.0f, 0);
                m_rumbleStatus = "Stopped";
                m_triggerStatus = "Stopped";
            }
            ImGui::SameLine();
            if (ImGui::Button("Close"))
            {
                m_showVibrationModal = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
	if (showDemo)
	{
		ImGui::ShowDemoWindow(&showDemo);
	}

    contentManager(*gamepad);
	// End Draw --------------
	ImGui::End();
}

void UI::applyModernTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	style.WindowRounding = 6.0f;       
	style.FrameRounding = 4.0f;        
	style.PopupRounding = 4.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabRounding = 4.0f;
	style.WindowPadding = ImVec2(15, 15);
	style.FramePadding = ImVec2(5, 5);
	style.ItemSpacing = ImVec2(10, 8);

	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f); 
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);

	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.30f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.25f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.28f, 0.30f, 1.00f);

	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
}

void UI::contentManager(Gamepad& gamepad)
{
    GamepadData gamepadData = gamepad.getData();
	if (gamepadData.connected)
	{
		drawContent(gamepad);
	}
	else {
		drawSearchContent();
	}
}

void UI::drawContent(Gamepad& gamepad)
{   
    GamepadData gamepadData = gamepad.getData();

    // Header
    float windowWidth = ImGui::GetWindowSize().x;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();

    std::string titleString = gamepadData.name + " | " + getGamepadType(gamepadData.type);
    const char* title = titleString.c_str();
    ImGui::SetCursorPosX((windowWidth - ImGui::CalcTextSize(title).x) * 0.5f);
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), title);
    ImGui::Separator();

    ImGui::Spacing();
    if (ImGui::BeginTable("GamepadLayout", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Visual", ImGuiTableColumnFlags_WidthStretch, 0.65f);
        ImGui::TableSetupColumn("Diagnostics", ImGuiTableColumnFlags_WidthStretch, 0.35f);
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::BeginChild("PadVisual", ImVec2(0, 520), true);
        {
            float visualWidth = ImGui::GetContentRegionAvail().x;
            ImDrawList* visualDrawList = ImGui::GetWindowDrawList();
            ImVec2 visualPos = ImGui::GetCursorScreenPos();
            float topY = visualPos.y + 10.0f;

            auto drawCircleBtn = [&](ImVec2 btnPos, const char* label, int btnIdx, ImColor activeColor) {
                bool active = gamepadData.buttons[btnIdx];
                visualDrawList->AddCircleFilled(btnPos, 15.0f, active ? activeColor : ImColor(45, 45, 45));
                visualDrawList->AddText(ImVec2(btnPos.x - 5, btnPos.y - 7), ImColor(255, 255, 255), label);
                };

            auto drawRectBtn = [&](ImVec2 btnPos, ImVec2 size, const char* label, int btnIdx) {
                bool active = gamepadData.buttons[btnIdx];
                visualDrawList->AddRectFilled(btnPos, ImVec2(btnPos.x + size.x, btnPos.y + size.y), active ? ImColor(0, 150, 255) : ImColor(45, 45, 45), 5.0f);
                visualDrawList->AddText(ImVec2(btnPos.x + 5, btnPos.y + 5), ImColor(255, 255, 255), label);
                };

            drawRectBtn(ImVec2(visualPos.x + visualWidth * 0.2f, topY + 20), ImVec2(80, 30), "L1", SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
            drawRectBtn(ImVec2(visualPos.x + visualWidth * 0.7f, topY + 20), ImVec2(80, 30), "R1", SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);

            ImGui::SetCursorScreenPos(ImVec2(visualPos.x + visualWidth * 0.2f, topY + 60));
            ImGui::ProgressBar(gamepadData.leftTrigger, ImVec2(80, 20), "L2");

            ImGui::SetCursorScreenPos(ImVec2(visualPos.x + visualWidth * 0.7f, topY + 60));
            ImGui::ProgressBar(gamepadData.rightTrigger, ImVec2(80, 20), "R2");

            float centerX = visualPos.x + visualWidth * 0.5f;
            drawCircleBtn(ImVec2(centerX - 40, topY + 100), "<", SDL_GAMEPAD_BUTTON_BACK, ImColor(150, 150, 150));
            drawRectBtn(ImVec2(centerX - 15, topY + 85), ImVec2(30, 30), "G", SDL_GAMEPAD_BUTTON_GUIDE);
            drawCircleBtn(ImVec2(centerX + 40, topY + 100), ">", SDL_GAMEPAD_BUTTON_START, ImColor(150, 150, 150));

            auto drawStick = [&](ImVec2 center, float x, float y, const char* label, int stickBtnIdx) {
                float radius = 45.0f;
                bool isPressed = gamepadData.buttons[stickBtnIdx];
                visualDrawList->AddCircleFilled(center, radius, isPressed ? ImColor(70, 70, 70) : ImColor(40, 40, 40));
                visualDrawList->AddCircle(center, radius, ImColor(100, 100, 100), 32, 2.0f);

                ImVec2 stickPos = ImVec2(center.x + (x * radius * 0.7f), center.y + (y * radius * 0.7f));
                visualDrawList->AddCircleFilled(stickPos, 12.0f, isPressed ? ImColor(0, 255, 255) : ImColor(0, 150, 255));
                visualDrawList->AddText(ImVec2(center.x - 25, center.y + radius + 10), ImColor(180, 180, 180), label);
                };

            std::string formatedLeftStickerAxis = std::format("[{} | {}]",
                floatToString(gamepadData.leftStick[0]),
                floatToString(gamepadData.leftStick[1]));
            std::string formatedRightStickerAxis = std::format("[{} | {}]",
                floatToString(gamepadData.rightStick[0]),
                floatToString(gamepadData.rightStick[1]));
            drawStick(ImVec2(visualPos.x + visualWidth * 0.3f, topY + 200),
                gamepadData.leftStick[0],
                gamepadData.leftStick[1],
                "L-Stick",
                SDL_GAMEPAD_BUTTON_LEFT_STICK);

            drawStick(ImVec2(visualPos.x + visualWidth * 0.7f, topY + 200),
                gamepadData.rightStick[0],
                gamepadData.rightStick[1],
                "R-Stick",
                SDL_GAMEPAD_BUTTON_RIGHT_STICK);

            float dpadX = visualPos.x + visualWidth * 0.15f;
            float dpadY = topY + 330;
            drawCircleBtn(ImVec2(dpadX, dpadY - 30), "U", SDL_GAMEPAD_BUTTON_DPAD_UP, ImColor(120, 120, 120));
            drawCircleBtn(ImVec2(dpadX, dpadY + 30), "D", SDL_GAMEPAD_BUTTON_DPAD_DOWN, ImColor(120, 120, 120));
            drawCircleBtn(ImVec2(dpadX - 30, dpadY), "L", SDL_GAMEPAD_BUTTON_DPAD_LEFT, ImColor(120, 120, 120));
            drawCircleBtn(ImVec2(dpadX + 30, dpadY), "R", SDL_GAMEPAD_BUTTON_DPAD_RIGHT, ImColor(120, 120, 120));

            float faceX = visualPos.x + visualWidth * 0.85f;
            float faceY = topY + 330;
            drawCircleBtn(ImVec2(faceX, faceY - 30), "Y", SDL_GAMEPAD_BUTTON_NORTH, ImColor(255, 255, 0));
            drawCircleBtn(ImVec2(faceX, faceY + 30), "A", SDL_GAMEPAD_BUTTON_SOUTH, ImColor(0, 255, 0));
            drawCircleBtn(ImVec2(faceX - 30, faceY), "X", SDL_GAMEPAD_BUTTON_WEST, ImColor(0, 150, 255));
            drawCircleBtn(ImVec2(faceX + 30, faceY), "B", SDL_GAMEPAD_BUTTON_EAST, ImColor(255, 0, 0));
        }
        ImGui::EndChild();

        ImGui::TableNextColumn();
        ImGui::BeginChild("PadDiagnostics", ImVec2(0, 520), true);
        {
            auto axisBar = [&](const char* label, float value) {
                float normalized = (value + 1.0f) * 0.5f;
                ImGui::Text("%s: %.2f", label, value);
                ImGui::ProgressBar(normalized, ImVec2(-1, 0));
            };

            ImGui::SeparatorText("Sticks");
            axisBar("LX", gamepadData.leftStick[0]);
            axisBar("LY", gamepadData.leftStick[1]);
            axisBar("RX", gamepadData.rightStick[0]);
            axisBar("RY", gamepadData.rightStick[1]);

            ImGui::Spacing();
            ImGui::SeparatorText("Triggers");
            ImGui::Text("L2: %.2f", gamepadData.leftTrigger);
            ImGui::ProgressBar(gamepadData.leftTrigger, ImVec2(-1, 0));
            ImGui::Text("R2: %.2f", gamepadData.rightTrigger);
            ImGui::ProgressBar(gamepadData.rightTrigger, ImVec2(-1, 0));

            ImGui::Spacing();
            ImGui::SeparatorText("Buttons");
            int pressedCount = 0;
            for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
                if (gamepadData.buttons[i]) {
                    pressedCount++;
                }
            }
            ImGui::Text("Pressed: %d", pressedCount);
            ImGui::BeginChild("PressedButtons", ImVec2(0, 140), true);
            for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
                if (gamepadData.buttons[i]) {
                    ImGui::BulletText("ID %d", i);
                }
            }
            ImGui::EndChild();

            ImGui::Spacing();
            ImGui::SeparatorText("Snapshot");
            ImGui::Text("Left Stick: [%.2f | %.2f]", gamepadData.leftStick[0], gamepadData.leftStick[1]);
            ImGui::Text("Right Stick: [%.2f | %.2f]", gamepadData.rightStick[0], gamepadData.rightStick[1]);
            ImGui::Text("Triggers: L2 %.2f, R2 %.2f", gamepadData.leftTrigger, gamepadData.rightTrigger);
        }
        ImGui::EndChild();

        ImGui::EndTable();
    }
}

void UI::drawSearchContent()
{
	ImVec2 availableSpace = ImGui::GetContentRegionAvail();

	ImGui::SetCursorPosX((availableSpace.x - 500) * 0.5f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (availableSpace.y - 35.0f) * 0.5f);
	ImGui::ProgressBar(-1.0 * (float)ImGui::GetTime(), ImVec2(500.0f,35.0f), "Searching Gamepad...");
}

std::string UI::floatToString(float value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string getGamepadType(SDL_GamepadType gamepadType)
{
    if (gamepadType == SDL_GAMEPAD_TYPE_XBOX360 | gamepadType == SDL_GAMEPAD_TYPE_XBOXONE)
    {
        return "Xbox";
    } else if (gamepadType == SDL_GAMEPAD_TYPE_PS5){
        return "Dualsense";
    } else if (gamepadType == SDL_GAMEPAD_TYPE_PS4){
        return "Dualshock 4";
    } else if (gamepadType == SDL_GAMEPAD_TYPE_PS4){
        return "Dualshock 3";
    } else if (gamepadType == SDL_GAMEPAD_TYPE_STANDARD){
        return "Generic";
    } else {
        return "Unknown";
    }
}
