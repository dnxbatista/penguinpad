#pragma once
#include "gamepad.h"
#include <SDL3/SDL.h>
#include <string>
class UI
{
public:
    void draw(bool& showDemo, Gamepad* gamepad);
    void applyModernTheme();
    bool loadTextures(SDL_Renderer* renderer);
    void cleanup();
private:
    void contentManager(Gamepad& gamepad);
    void drawContent(Gamepad& gamepad);
    void drawSearchContent();
    
    void drawMenuBar(Gamepad* gamepad);
    void drawLightbarModal(Gamepad* gamepad);
    void drawVibrationModal(Gamepad* gamepad);
    void drawPrecisionModal(Gamepad* gamepad);
    void drawGyroModal(Gamepad* gamepad);
    void drawTouchpadModal(Gamepad* gamepad);

    std::string floatToString(float value);

    float m_lightbarColor[3] = { 0.0f, 0.5f, 1.0f };
    bool m_showLightbarModal = false;
    bool m_showVibrationModal = false;
    bool m_showPrecisionModal = false;
    bool m_showGyroModal = false;
    bool m_showTouchpadModal = false;

    float m_rumbleLow = 0.6f;
    float m_rumbleHigh = 0.8f;
    float m_triggerLeft = 0.6f;
    float m_triggerRight = 0.6f;
    int m_rumbleDurationMs = 300;
    std::string m_rumbleStatus;
    std::string m_triggerStatus;

    float m_precisionDeadzone = 0.08f;
    float m_leftCenter[2] = { 0.0f, 0.0f };
    float m_rightCenter[2] = { 0.0f, 0.0f };
    bool m_precisionTestActive = false;
    float m_precisionTimer = 0.0f;
    int m_precisionSamples = 0;
    float m_leftDriftSum = 0.0f;
    float m_rightDriftSum = 0.0f;
    float m_leftMaxDrift = 0.0f;
    float m_rightMaxDrift = 0.0f;
    float m_leftAvgDrift = 0.0f;
    float m_rightAvgDrift = 0.0f;

    bool m_gyroEnabled = true;
    float m_gyroOffset[3] = { 0.0f, 0.0f, 0.0f };
    float m_gyroData[3] = { 0.0f, 0.0f, 0.0f };
    std::string m_gyroStatus;
    
    // Gyro visualization
    float m_gyroSensitivity = 1.5f;
    bool m_gyroChallengeMode = false;
    float m_gyroChallengeTimer = 0.0f;
    float m_gyroChallengeZone = 0.15f;
    float m_gyroStability = 0.0f;
    static constexpr int kGyroTrailSize = 10;
    float m_gyroTrailX[kGyroTrailSize] = {};
    float m_gyroTrailY[kGyroTrailSize] = {};
    int m_gyroTrailIndex = 0;

    SDL_Texture* m_arrowTexture = nullptr;
};



