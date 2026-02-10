#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <cstdint>

struct GamepadData {
	bool connected = false;
	std::string name = "None";
	SDL_GamepadType type = SDL_GAMEPAD_TYPE_UNKNOWN;

	bool buttons[SDL_GAMEPAD_BUTTON_COUNT] = { false };

	float leftStick[2] = { 0.0f, 0.0f };
	float rightStick[2] = { 0.0f, 0.0f };

	float leftTrigger = 0.0f;
	float rightTrigger = 0.0f;
};

class Gamepad {
public:
	Gamepad();
	~Gamepad();

	void update();
	void handleEvent(const SDL_Event& event);

	void setLightbar(uint8_t r, uint8_t g, uint8_t b);
	bool rumble(float lowFrequency, float highFrequency, uint32_t durationMs);
	bool rumbleTriggers(float left, float right, uint32_t durationMs);
	bool hasGyro() const;
	bool setGyroEnabled(bool enabled);
	bool gyroEnabled() const;
	bool getGyro(float out[3]) const;
	float gyroRate() const;

	const GamepadData& getData() const { return m_data; }
private:
	// SDL
	SDL_Gamepad* m_SDLgamepad = nullptr;
	GamepadData m_data;

	void openFirstAvailable();
	void close();
	float normalizeAxis(Sint16 value);
};