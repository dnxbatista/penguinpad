#include "ui.h"
#include "gamepad.h"
#include <imgui.h>
#include <cmath>
#include <string>
#include <sstream>
#include <format>
#include <cstdint>
#include <iostream>

namespace {

std::string getGamepadType(SDL_GamepadType gamepadType)
{
    if (gamepadType == SDL_GAMEPAD_TYPE_XBOX360 || gamepadType == SDL_GAMEPAD_TYPE_XBOXONE)
        return "Xbox";
    if (gamepadType == SDL_GAMEPAD_TYPE_PS5)
        return "Dualsense";
    if (gamepadType == SDL_GAMEPAD_TYPE_PS4)
        return "Dualshock 4";
    if (gamepadType == SDL_GAMEPAD_TYPE_PS3)
        return "Dualshock 3";
    if (gamepadType == SDL_GAMEPAD_TYPE_STANDARD)
        return "Generic";
    return "Unknown";
}

}

void UI::draw(bool& showDemo, Gamepad* gamepad)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar;

    ImGui::Begin("penguinpad", nullptr, flags);
    
    drawMenuBar(gamepad);
    drawLightbarModal(gamepad);
    drawVibrationModal(gamepad);
    drawPrecisionModal(gamepad);
    drawGyroModal(gamepad);
    
    if (showDemo)
        ImGui::ShowDemoWindow(&showDemo);

    contentManager(*gamepad);
    ImGui::End();
}

void UI::drawMenuBar(Gamepad* gamepad)
{
    if (gamepad->getData().connected && ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Gamepad Light"))
                m_showLightbarModal = true;
            if (ImGui::MenuItem("Gamepad Vibration"))
                m_showVibrationModal = true;
            if (ImGui::MenuItem("Joystick Precision"))
                m_showPrecisionModal = true;
            if (ImGui::MenuItem("Gamepad Gyro"))
                m_showGyroModal = true;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void UI::drawLightbarModal(Gamepad* gamepad)
{
    if (!m_showLightbarModal)
        return;
        
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

void UI::drawVibrationModal(Gamepad* gamepad)
{
    if (!m_showVibrationModal)
        return;
        
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

void UI::drawPrecisionModal(Gamepad* gamepad)
{
    if (!m_showPrecisionModal)
        return;
        
    ImGui::OpenPopup("Joystick Precision");
    if (ImGui::BeginPopupModal("Joystick Precision", &m_showPrecisionModal, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Joystick Precision Tester");
        ImGui::Separator();

        ImGui::Text("Deadzone");
        ImGui::SliderFloat("##PrecisionDeadzone", &m_precisionDeadzone, 0.0f, 0.3f, "%.2f");

        ImGui::Spacing();
        if (ImGui::Button("Calibrate Left Center"))
        {
            m_leftCenter[0] = gamepad->getData().leftStick[0];
            m_leftCenter[1] = gamepad->getData().leftStick[1];
        }
        ImGui::SameLine();
        if (ImGui::Button("Calibrate Right Center"))
        {
            m_rightCenter[0] = gamepad->getData().rightStick[0];
            m_rightCenter[1] = gamepad->getData().rightStick[1];
        }

        ImGui::Spacing();
        if (ImGui::BeginTable("PrecisionLayout", 2, ImGuiTableFlags_SizingStretchProp))
        {
            auto drawStickPanel = [&](const char* label, float x, float y) {
                ImGui::BeginChild(label, ImVec2(0, 180), true);
                ImVec2 panelPos = ImGui::GetCursorScreenPos();
                ImVec2 panelSize = ImGui::GetContentRegionAvail();
                float size = (panelSize.x < panelSize.y) ? panelSize.x : panelSize.y;
                float radius = (size * 0.5f) - 10.0f;
                ImVec2 center = ImVec2(panelPos.x + panelSize.x * 0.5f, panelPos.y + panelSize.y * 0.5f - 8.0f);

                ImDrawList* list = ImGui::GetWindowDrawList();
                list->AddCircle(center, radius, ImColor(120, 120, 120), 48, 2.0f);
                list->AddLine(ImVec2(center.x - radius, center.y), ImVec2(center.x + radius, center.y), ImColor(80, 80, 80), 1.0f);
                list->AddLine(ImVec2(center.x, center.y - radius), ImVec2(center.x, center.y + radius), ImColor(80, 80, 80), 1.0f);

                ImVec2 dot = ImVec2(center.x + x * radius, center.y + y * radius);
                list->AddCircleFilled(dot, 6.0f, ImColor(0, 180, 255));

                ImGui::Text("%s", label);
                ImGui::EndChild();
            };

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            float leftX = gamepad->getData().leftStick[0] - m_leftCenter[0];
            float leftY = gamepad->getData().leftStick[1] - m_leftCenter[1];
            drawStickPanel("Left Stick", leftX, leftY);

            ImGui::TableNextColumn();
            float rightX = gamepad->getData().rightStick[0] - m_rightCenter[0];
            float rightY = gamepad->getData().rightStick[1] - m_rightCenter[1];
            drawStickPanel("Right Stick", rightX, rightY);

            ImGui::EndTable();
        }

        float leftMagnitude = std::sqrt(
            (gamepad->getData().leftStick[0] - m_leftCenter[0]) * (gamepad->getData().leftStick[0] - m_leftCenter[0]) +
            (gamepad->getData().leftStick[1] - m_leftCenter[1]) * (gamepad->getData().leftStick[1] - m_leftCenter[1]));
        float rightMagnitude = std::sqrt(
            (gamepad->getData().rightStick[0] - m_rightCenter[0]) * (gamepad->getData().rightStick[0] - m_rightCenter[0]) +
            (gamepad->getData().rightStick[1] - m_rightCenter[1]) * (gamepad->getData().rightStick[1] - m_rightCenter[1]));

        ImGui::SeparatorText("Metrics");
        ImGui::Text("Left: [%.3f | %.3f]  Magnitude: %.3f  %s",
            gamepad->getData().leftStick[0] - m_leftCenter[0],
            gamepad->getData().leftStick[1] - m_leftCenter[1],
            leftMagnitude,
            (leftMagnitude <= m_precisionDeadzone) ? "In Deadzone" : "Active");
        ImGui::Text("Right: [%.3f | %.3f]  Magnitude: %.3f  %s",
            gamepad->getData().rightStick[0] - m_rightCenter[0],
            gamepad->getData().rightStick[1] - m_rightCenter[1],
            rightMagnitude,
            (rightMagnitude <= m_precisionDeadzone) ? "In Deadzone" : "Active");

        ImGui::Spacing();
        if (ImGui::Button(m_precisionTestActive ? "Testing..." : "Start Hold Still Test"))
        {
            if (!m_precisionTestActive)
            {
                m_precisionTestActive = true;
                m_precisionTimer = 0.0f;
                m_precisionSamples = 0;
                m_leftDriftSum = 0.0f;
                m_rightDriftSum = 0.0f;
                m_leftMaxDrift = 0.0f;
                m_rightMaxDrift = 0.0f;
                m_leftAvgDrift = 0.0f;
                m_rightAvgDrift = 0.0f;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Stats"))
        {
            m_precisionTestActive = false;
            m_precisionTimer = 0.0f;
            m_precisionSamples = 0;
            m_leftDriftSum = 0.0f;
            m_rightDriftSum = 0.0f;
            m_leftMaxDrift = 0.0f;
            m_rightMaxDrift = 0.0f;
            m_leftAvgDrift = 0.0f;
            m_rightAvgDrift = 0.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            m_showPrecisionModal = false;
            ImGui::CloseCurrentPopup();
        }

        if (m_precisionTestActive)
        {
            m_precisionTimer += ImGui::GetIO().DeltaTime;
            float leftDrift = leftMagnitude;
            float rightDrift = rightMagnitude;

            if (leftDrift > m_leftMaxDrift) m_leftMaxDrift = leftDrift;
            if (rightDrift > m_rightMaxDrift) m_rightMaxDrift = rightDrift;

            m_leftDriftSum += leftDrift;
            m_rightDriftSum += rightDrift;
            m_precisionSamples++;

            if (m_precisionTimer >= 2.0f)
            {
                m_precisionTestActive = false;
                if (m_precisionSamples > 0)
                {
                    m_leftAvgDrift = m_leftDriftSum / static_cast<float>(m_precisionSamples);
                    m_rightAvgDrift = m_rightDriftSum / static_cast<float>(m_precisionSamples);
                }
            }
        }

        ImGui::Spacing();
        ImGui::SeparatorText("Hold Still Results (2s)");
        ImGui::Text("Left  Avg: %.4f  Max: %.4f", m_leftAvgDrift, m_leftMaxDrift);
        ImGui::Text("Right Avg: %.4f  Max: %.4f", m_rightAvgDrift, m_rightMaxDrift);
        if (m_precisionTestActive)
        {
            float remaining = 2.0f - m_precisionTimer;
            if (remaining < 0.0f) remaining = 0.0f;
            ImGui::Text("Time Remaining: %.1f s", remaining);
        }

        ImGui::EndPopup();
    }
}

void UI::drawGyroModal(Gamepad* gamepad)
{
    if (!m_showGyroModal)
        return;
        
    ImGui::OpenPopup("Gamepad Gyro");
    if (ImGui::BeginPopupModal("Gamepad Gyro", &m_showGyroModal, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Gyro Tester");
        ImGui::Separator();

        bool gyroSupported = gamepad->hasGyro();
        if (!gyroSupported)
        {
            ImGui::TextDisabled("Gyro not supported on this gamepad.");
        }
        else
        {
            bool prevEnabled = m_gyroEnabled;
            ImGui::Checkbox("Enable Gyro", &m_gyroEnabled);
            if (m_gyroEnabled != prevEnabled)
            {
                bool ok = gamepad->setGyroEnabled(m_gyroEnabled);
                m_gyroStatus = ok ? "Gyro updated" : "Failed to toggle gyro";
            }

            if (gamepad->gyroEnabled())
            {
                if (gamepad->getGyro(m_gyroData))
                {
                    float gx = m_gyroData[0] - m_gyroOffset[0];
                    float gy = m_gyroData[1] - m_gyroOffset[1];
                    float gz = m_gyroData[2] - m_gyroOffset[2];
                    float magnitude = std::sqrt((gx * gx) + (gy * gy) + (gz * gz));

                    float posX = gy * m_gyroSensitivity;
                    float posY = -gx * m_gyroSensitivity;
                    
                    posX = std::clamp(posX, -1.0f, 1.0f);
                    posY = std::clamp(posY, -1.0f, 1.0f);

                    m_gyroTrailX[m_gyroTrailIndex] = posX;
                    m_gyroTrailY[m_gyroTrailIndex] = posY;
                    m_gyroTrailIndex = (m_gyroTrailIndex + 1) % kGyroTrailSize;

                    float distance = std::sqrt((posX * posX) + (posY * posY));
                    m_gyroStability = 1.0f - (distance < 1.0f ? distance : 1.0f);

                    if (ImGui::BeginTable("GyroLayout", 2, ImGuiTableFlags_SizingStretchProp))
                    {
                        ImGui::TableSetupColumn("Visual", ImGuiTableColumnFlags_WidthStretch, 0.6f);
                        ImGui::TableSetupColumn("Info", ImGuiTableColumnFlags_WidthStretch, 0.4f);
                        ImGui::TableNextRow();

                        ImGui::TableNextColumn();
                        ImGui::BeginChild("GyroVisual", ImVec2(320, 320), true);
                        {
                            ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
                            ImVec2 canvas_size = ImGui::GetContentRegionAvail();
                            float boxSize = (canvas_size.x < canvas_size.y ? canvas_size.x : canvas_size.y) - 20.0f;
                            ImVec2 center = ImVec2(canvas_pos.x + canvas_size.x * 0.5f, canvas_pos.y + canvas_size.y * 0.5f);
                            float halfBox = boxSize * 0.5f;

                            ImDrawList* draw = ImGui::GetWindowDrawList();
                            
                            draw->AddRectFilled(
                                ImVec2(center.x - halfBox, center.y - halfBox),
                                ImVec2(center.x + halfBox, center.y + halfBox),
                                ImColor(30, 30, 35), 8.0f);
                            
                            draw->AddRect(
                                ImVec2(center.x - halfBox, center.y - halfBox),
                                ImVec2(center.x + halfBox, center.y + halfBox),
                                ImColor(100, 100, 100), 8.0f, 0, 3.0f);

                            if (m_gyroChallengeMode)
                            {
                                float safeZoneRadius = halfBox * m_gyroChallengeZone;
                                draw->AddCircle(center, safeZoneRadius, ImColor(0, 255, 100, 100), 48, 2.0f);
                                
                                if (distance * halfBox <= safeZoneRadius)
                                {
                                    draw->AddCircleFilled(center, safeZoneRadius, ImColor(0, 255, 100, 30));
                                }
                            }

                            for (int i = 0; i < kGyroTrailSize; i++)
                            {
                                int idx = (m_gyroTrailIndex - i - 1 + kGyroTrailSize) % kGyroTrailSize;
                                float alpha = (1.0f - (i / (float)kGyroTrailSize)) * 0.5f;
                                ImVec2 trailPos = ImVec2(
                                    center.x + m_gyroTrailX[idx] * halfBox,
                                    center.y + m_gyroTrailY[idx] * halfBox
                                );
                                draw->AddCircleFilled(trailPos, 3.0f, ImColor(0.0f, 0.7f, 1.0f, alpha));
                            }

                            ImVec2 dotPos = ImVec2(center.x + posX * halfBox, center.y + posY * halfBox);
                            draw->AddCircleFilled(dotPos, 12.0f, ImColor(0, 180, 255));
                            draw->AddCircle(dotPos, 12.0f, ImColor(255, 255, 255), 16, 2.0f);
                            
                            draw->AddLine(center, dotPos, ImColor(150, 150, 150, 100), 1.5f);

                            draw->AddLine(
                                ImVec2(center.x - halfBox, center.y),
                                ImVec2(center.x + halfBox, center.y),
                                ImColor(80, 80, 80), 1.0f);
                            draw->AddLine(
                                ImVec2(center.x, center.y - halfBox),
                                ImVec2(center.x, center.y + halfBox),
                                ImColor(80, 80, 80), 1.0f);

                            ImGui::SetCursorScreenPos(ImVec2(canvas_pos.x + 5, canvas_pos.y + 5));
                            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Tilt Visualization");
                        }
                        ImGui::EndChild();

                        ImGui::TableNextColumn();
                        ImGui::BeginChild("GyroInfo", ImVec2(0, 320), true);
                        {
                            ImGui::SeparatorText("Data");
                            ImGui::Text("X: %.3f", gx);
                            ImGui::Text("Y: %.3f", gy);
                            ImGui::Text("Z: %.3f", gz);
                            ImGui::Text("Magnitude: %.2f", magnitude);
                            
                            ImGui::Spacing();
                            ImGui::SeparatorText("Stability");
                            ImGui::ProgressBar(m_gyroStability, ImVec2(-1, 0));
                            
                            if (m_gyroChallengeMode)
                            {
                                ImGui::Spacing();
                                ImGui::SeparatorText("Challenge");
                                m_gyroChallengeTimer += ImGui::GetIO().DeltaTime;
                                float dist = std::sqrt((posX * posX) + (posY * posY));
                                bool inZone = dist <= m_gyroChallengeZone;
                                
                                if (inZone)
                                {
                                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.3f, 1.0f), "HOLD STEADY!");
                                }
                                else
                                {
                                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.0f, 1.0f), "OUT OF ZONE");
                                }
                                ImGui::Text("Time: %.1fs", m_gyroChallengeTimer);
                            }

                            ImGui::Spacing();
                            ImGui::SeparatorText("Settings");
                            ImGui::SliderFloat("Sensitivity", &m_gyroSensitivity, 0.5f, 3.0f, "%.1f");
                            
                            if (m_gyroChallengeMode)
                            {
                                ImGui::SliderFloat("Zone Size", &m_gyroChallengeZone, 0.1f, 0.5f, "%.2f");
                            }
                        }
                        ImGui::EndChild();

                        ImGui::EndTable();
                    }

                    ImGui::Spacing();
                    if (ImGui::Button(m_gyroChallengeMode ? "Stop Challenge" : "Start Challenge"))
                    {
                        m_gyroChallengeMode = !m_gyroChallengeMode;
                        m_gyroChallengeTimer = 0.0f;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Zero Offset"))
                    {
                        if (gamepad->getGyro(m_gyroOffset))
                        {
                            m_gyroStatus = "Offset saved";
                            m_gyroTrailIndex = 0;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Reset Offset"))
                    {
                        m_gyroOffset[0] = 0.0f;
                        m_gyroOffset[1] = 0.0f;
                        m_gyroOffset[2] = 0.0f;
                        m_gyroStatus = "Offset cleared";
                        m_gyroTrailIndex = 0;
                    }
                }
                else
                {
                    ImGui::TextDisabled("No gyro data available.");
                }
            }
            else
            {
                ImGui::TextDisabled("Gyro is disabled.");
            }
        }

        if (!m_gyroStatus.empty())
        {
            ImGui::TextDisabled("%s", m_gyroStatus.c_str());
        }

        ImGui::Spacing();
        if (ImGui::Button("Close"))
        {
            m_showGyroModal = false;
            m_gyroChallengeMode = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
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
    if (gamepad.getData().connected)
        drawContent(gamepad);
    else
        drawSearchContent();
}

void UI::drawContent(Gamepad& gamepad)
{
    GamepadData gamepadData = gamepad.getData();

    float windowWidth = ImGui::GetWindowSize().x;
    std::string titleString = gamepadData.name + " | " + getGamepadType(gamepadData.type);
    ImGui::SetCursorPosX((windowWidth - ImGui::CalcTextSize(titleString.c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "%s", titleString.c_str());
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
                visualDrawList->AddRectFilled(btnPos, ImVec2(btnPos.x + size.x, btnPos.y + size.y), 
                    active ? ImColor(0, 150, 255) : ImColor(45, 45, 45), 5.0f);
                visualDrawList->AddText(ImVec2(btnPos.x + 5, btnPos.y + 5), ImColor(255, 255, 255), label);
            };

            auto drawStick = [&](ImVec2 center, float x, float y, const char* label, int stickBtnIdx) {
                float radius = 45.0f;
                bool isPressed = gamepadData.buttons[stickBtnIdx];
                visualDrawList->AddCircleFilled(center, radius, isPressed ? ImColor(70, 70, 70) : ImColor(40, 40, 40));
                visualDrawList->AddCircle(center, radius, ImColor(100, 100, 100), 32, 2.0f);

                ImVec2 stickPos = ImVec2(center.x + (x * radius * 0.7f), center.y + (y * radius * 0.7f));
                visualDrawList->AddCircleFilled(stickPos, 12.0f, isPressed ? ImColor(0, 255, 255) : ImColor(0, 150, 255));
                visualDrawList->AddText(ImVec2(center.x - 25, center.y + radius + 10), ImColor(180, 180, 180), label);
            };

            auto drawArrowBtn = [&](ImVec2 center, int btnIdx, float rotationDeg) {
                bool active = gamepadData.buttons[btnIdx];
                float size = 24.0f;
                float halfSize = size * 0.5f;
                ImColor tint = active ? ImColor(0, 180, 255) : ImColor(120, 120, 120);
                
                float angleRad = rotationDeg * 3.14159265f / 180.0f;
                float cosA = std::cos(angleRad);
                float sinA = std::sin(angleRad);
                
                ImVec2 corners[4] = {
                    ImVec2(-halfSize, -halfSize), ImVec2(halfSize, -halfSize),
                    ImVec2(halfSize, halfSize), ImVec2(-halfSize, halfSize)
                };
                
                for (int i = 0; i < 4; i++) {
                    float rx = corners[i].x * cosA - corners[i].y * sinA;
                    float ry = corners[i].x * sinA + corners[i].y * cosA;
                    corners[i] = ImVec2(center.x + rx, center.y + ry);
                }
                
                visualDrawList->AddImageQuad(m_arrowTexture, corners[0], corners[1], corners[2], corners[3],
                    ImVec2(0, 0), ImVec2(1, 0), ImVec2(1, 1), ImVec2(0, 1),
                    ImGui::ColorConvertFloat4ToU32(ImVec4(tint.Value.x, tint.Value.y, tint.Value.z, tint.Value.w)));
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

            drawStick(ImVec2(visualPos.x + visualWidth * 0.3f, topY + 200),
                gamepadData.leftStick[0], gamepadData.leftStick[1], "L-Stick", SDL_GAMEPAD_BUTTON_LEFT_STICK);
            drawStick(ImVec2(visualPos.x + visualWidth * 0.7f, topY + 200),
                gamepadData.rightStick[0], gamepadData.rightStick[1], "R-Stick", SDL_GAMEPAD_BUTTON_RIGHT_STICK);

            float dpadX = visualPos.x + visualWidth * 0.15f;
            float dpadY = topY + 330;
            
            if (m_arrowTexture)
            {
                drawArrowBtn(ImVec2(dpadX, dpadY - 30), SDL_GAMEPAD_BUTTON_DPAD_UP, 0.0f);
                drawArrowBtn(ImVec2(dpadX, dpadY + 30), SDL_GAMEPAD_BUTTON_DPAD_DOWN, 180.0f);
                drawArrowBtn(ImVec2(dpadX - 30, dpadY), SDL_GAMEPAD_BUTTON_DPAD_LEFT, 270.0f);
                drawArrowBtn(ImVec2(dpadX + 30, dpadY), SDL_GAMEPAD_BUTTON_DPAD_RIGHT, 90.0f);
            }
            else
            {
                drawCircleBtn(ImVec2(dpadX, dpadY - 30), "U", SDL_GAMEPAD_BUTTON_DPAD_UP, ImColor(120, 120, 120));
                drawCircleBtn(ImVec2(dpadX, dpadY + 30), "D", SDL_GAMEPAD_BUTTON_DPAD_DOWN, ImColor(120, 120, 120));
                drawCircleBtn(ImVec2(dpadX - 30, dpadY), "L", SDL_GAMEPAD_BUTTON_DPAD_LEFT, ImColor(120, 120, 120));
                drawCircleBtn(ImVec2(dpadX + 30, dpadY), "R", SDL_GAMEPAD_BUTTON_DPAD_RIGHT, ImColor(120, 120, 120));
            }

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
            for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
                if (gamepadData.buttons[i]) pressedCount++;
            
            ImGui::Text("Pressed: %d", pressedCount);
            ImGui::BeginChild("PressedButtons", ImVec2(0, 140), true);
            for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
                if (gamepadData.buttons[i]) ImGui::BulletText("ID %d", i);
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
    ImGui::ProgressBar(-1.0 * (float)ImGui::GetTime(), ImVec2(500.0f, 35.0f), "Searching Gamepad...");
}

std::string UI::floatToString(float value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

bool UI::loadTextures(SDL_Renderer* renderer)
{
    SDL_Surface* surface = SDL_LoadBMP("../assets/images/arrow.bmp");
    if (!surface)
    {
        std::cerr << "Failed to load arrow.bmp: " << SDL_GetError() << std::endl;
        return false;
    }

    m_arrowTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!m_arrowTexture)
    {
        std::cerr << "Failed to create arrow texture: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void UI::cleanup()
{
    if (m_arrowTexture)
    {
        SDL_DestroyTexture(m_arrowTexture);
        m_arrowTexture = nullptr;
    }
}
