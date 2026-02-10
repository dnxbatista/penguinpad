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

    float contentHeight = 450.0f;
    float availHeight = ImGui::GetContentRegionAvail().y;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (availHeight - contentHeight) * 0.5f);

    // Gamepad UI
    ImGui::BeginGroup();
    {
        // --- (LAMBDAS) ---
        auto drawCircleBtn = [&](ImVec2 pos, const char* label, int btnIdx, ImColor activeColor) {
            bool active = gamepadData.buttons[btnIdx];
            drawList->AddCircleFilled(pos, 15.0f, active ? activeColor : ImColor(45, 45, 45));
            drawList->AddText(ImVec2(pos.x - 5, pos.y - 7), ImColor(255, 255, 255), label);
            };

        auto drawRectBtn = [&](ImVec2 pos, ImVec2 size, const char* label, int btnIdx) {
            bool active = gamepadData.buttons[btnIdx];
            drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), active ? ImColor(0, 150, 255) : ImColor(45, 45, 45), 5.0f);
            drawList->AddText(ImVec2(pos.x + 5, pos.y + 5), ImColor(255, 255, 255), label);
            };

        drawRectBtn(ImVec2(pos.x + windowWidth * 0.2f, pos.y + 40), ImVec2(80, 30), "L1", SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
        drawRectBtn(ImVec2(pos.x + windowWidth * 0.7f, pos.y + 40), ImVec2(80, 30), "R1", SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);

        ImGui::SetCursorScreenPos(ImVec2(pos.x + windowWidth * 0.2f, pos.y + 80));
        ImGui::ProgressBar(gamepadData.leftTrigger, ImVec2(80, 20), "L2");
        drawList->AddText(ImVec2(pos.x + windowWidth * 0.2f, pos.y + 110),
         ImColor(180, 180, 180), 
         floatToString(gamepadData.leftTrigger).c_str());

        ImGui::SetCursorScreenPos(ImVec2(pos.x + windowWidth * 0.7f, pos.y + 80));
        ImGui::ProgressBar(gamepadData.rightTrigger, ImVec2(80, 20), "R2");
        drawList->AddText(ImVec2(pos.x + windowWidth * 0.7f, pos.y + 110), 
        ImColor(180, 180, 180), 
        floatToString(gamepadData.rightTrigger).c_str());

        float centerX = pos.x + windowWidth * 0.5f;
        drawCircleBtn(ImVec2(centerX - 40, pos.y + 120), "<", SDL_GAMEPAD_BUTTON_BACK, ImColor(150, 150, 150));
        drawRectBtn(ImVec2(centerX - 15, pos.y + (120- 15)), ImVec2(30, 30), "G", SDL_GAMEPAD_BUTTON_GUIDE);
        drawCircleBtn(ImVec2(centerX + 40, pos.y + 120), ">", SDL_GAMEPAD_BUTTON_START, ImColor(150, 150, 150));

        auto drawStick = [&](ImVec2 center, float x, float y, const char* label, const char* stickValue, int stickBtnIdx) {
            float radius = 45.0f;
            bool isPressed = gamepadData.buttons[stickBtnIdx];
            drawList->AddCircleFilled(center, radius, isPressed ? ImColor(70, 70, 70) : ImColor(40, 40, 40));
            drawList->AddCircle(center, radius, ImColor(100, 100, 100), 32, 2.0f);

            ImVec2 stickPos = ImVec2(center.x + (x * radius * 0.7f), center.y + (y * radius * 0.7f));
            drawList->AddCircleFilled(stickPos, 12.0f, isPressed ? ImColor(0, 255, 255) : ImColor(0, 150, 255));
            drawList->AddText(ImVec2(center.x - 25, center.y + radius - 115), ImColor(180, 180, 180), label);
            drawList->AddText(ImVec2(center.x - 85, center.y + radius + 10), ImColor(180, 180, 180), stickValue);
            };
        
        std::string formatedLeftStickerAxis = std::format("[{} | {}]", 
            floatToString(gamepadData.leftStick[0]), 
            floatToString(gamepadData.leftStick[1]));
        std::string formatedRightStickerAxis = std::format("[{} | {}]", 
            floatToString(gamepadData.rightStick[0]), 
            floatToString(gamepadData.rightStick[1]));
        drawStick(ImVec2(pos.x + windowWidth * 0.3f, pos.y + 220), 
        gamepadData.leftStick[0], 
        gamepadData.leftStick[1], 
        "L-Stick", 
        formatedLeftStickerAxis.c_str(), 
        SDL_GAMEPAD_BUTTON_LEFT_STICK);

        drawStick(ImVec2(pos.x + windowWidth * 0.7f, pos.y + 220), 
        gamepadData.rightStick[0], 
        gamepadData.rightStick[1], 
        "R-Stick",
        formatedRightStickerAxis.c_str(),
        SDL_GAMEPAD_BUTTON_RIGHT_STICK);

        float dpadX = pos.x + windowWidth * 0.15f;
        float dpadY = pos.y + 350;
        drawCircleBtn(ImVec2(dpadX, dpadY - 30), "U", SDL_GAMEPAD_BUTTON_DPAD_UP, ImColor(120, 120, 120));
        drawCircleBtn(ImVec2(dpadX, dpadY + 30), "D", SDL_GAMEPAD_BUTTON_DPAD_DOWN, ImColor(120, 120, 120));
        drawCircleBtn(ImVec2(dpadX - 30, dpadY), "L", SDL_GAMEPAD_BUTTON_DPAD_LEFT, ImColor(120, 120, 120));
        drawCircleBtn(ImVec2(dpadX + 30, dpadY), "R", SDL_GAMEPAD_BUTTON_DPAD_RIGHT, ImColor(120, 120, 120));

        float faceX = windowWidth * 0.85f;
        float faceY = pos.y + 350;
        drawCircleBtn(ImVec2(faceX, faceY - 30), "Y", SDL_GAMEPAD_BUTTON_NORTH, ImColor(255, 255, 0));
        drawCircleBtn(ImVec2(faceX, faceY + 30), "A", SDL_GAMEPAD_BUTTON_SOUTH, ImColor(0, 255, 0));
        drawCircleBtn(ImVec2(faceX - 30, faceY), "X", SDL_GAMEPAD_BUTTON_WEST, ImColor(0, 150, 255));
        drawCircleBtn(ImVec2(faceX + 30, faceY), "B", SDL_GAMEPAD_BUTTON_EAST, ImColor(255, 0, 0));
    }
    ImGui::EndGroup();

    // Buttons Diagnosis
    ImGui::SetCursorPosY(pos.y + 480);
    ImGui::SeparatorText("Extras Buttons / Diagnosis");
    ImGui::BeginChild("ExtraBtns", ImVec2(0, 100), true);
    for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++) {
        if (gamepadData.buttons[i]) {
            ImGui::Text("ID [%d] Pressed", i);
            ImGui::SameLine();
        }
    }
    ImGui::EndChild();
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
