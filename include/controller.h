#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>

struct GamepadData {
	bool connected = false;
	std::string name = "None";

	bool buttons[SDL_GAMEPAD_BUTTON_COUNT] = { false };

	float leftStick[2] = { 0.0f, 0.0f };
	float rightStick[2] = { 0.0f, 0.0f };

	float leftTrigger = 0.0f;
	float rightTrigger = 0.0f;
};

class Controller {
public:
	Controller();
	~Controller();

	void update();
	void handleEvent(const SDL_Event& event);

	const GamepadData& getData() const { return m_data; }
private:
	SDL_Gamepad* m_gamepad = nullptr;
	GamepadData m_data;

	void openFirstAvailable();
	void close();
	float normalizeAxis(Sint16 value);
};