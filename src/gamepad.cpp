#include "gamepad.h"
#include <iostream>
#include <random>
#include <cstdint>

Gamepad::Gamepad()
{
	openFirstAvailable();
}

Gamepad::~Gamepad()
{
	close();
}

void Gamepad::update()
{
	SDL_UpdateGamepads();
	if (!m_SDLgamepad) return;

	for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i) {
		m_data.buttons[i] = SDL_GetGamepadButton(m_SDLgamepad, (SDL_GamepadButton)i);
	}

	m_data.leftStick[0] = normalizeAxis(SDL_GetGamepadAxis(m_SDLgamepad, SDL_GAMEPAD_AXIS_LEFTX));
	m_data.leftStick[1] = normalizeAxis(SDL_GetGamepadAxis(m_SDLgamepad, SDL_GAMEPAD_AXIS_LEFTY));
	m_data.rightStick[0] = normalizeAxis(SDL_GetGamepadAxis(m_SDLgamepad, SDL_GAMEPAD_AXIS_RIGHTX));
	m_data.rightStick[1] = normalizeAxis(SDL_GetGamepadAxis(m_SDLgamepad, SDL_GAMEPAD_AXIS_RIGHTY));

	m_data.leftTrigger = normalizeAxis(SDL_GetGamepadAxis(m_SDLgamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));
	m_data.rightTrigger = normalizeAxis(SDL_GetGamepadAxis(m_SDLgamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));
}

void Gamepad::handleEvent(const SDL_Event& event)
{
	// Gamepad Events
	if (event.type == SDL_EVENT_GAMEPAD_ADDED) {
		if (!m_SDLgamepad) openFirstAvailable();
	}
	else if (event.type == SDL_EVENT_GAMEPAD_REMOVED) {
		if (m_SDLgamepad && event.gdevice.which == SDL_GetGamepadID(m_SDLgamepad)) {
			close();
		}
	}

	if (event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN){
		if (event.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT){
			uint8_t random_byte1 = static_cast<uint8_t>(std::rand() & 0xFF);
			uint8_t random_byte2 = static_cast<uint8_t>(std::rand() & 0xFF);
			uint8_t random_byte3 = static_cast<uint8_t>(std::rand() & 0xFF);
			setLightbar(random_byte1, random_byte2, random_byte3);
		}
	}
}

void Gamepad::setLightbar(uint8_t r, uint8_t g, uint8_t b)
{
	if (m_SDLgamepad) {
		SDL_SetGamepadLED(m_SDLgamepad, r, g, b);
	}
}

void Gamepad::openFirstAvailable()
{
	int count;
	SDL_JoystickID* joysticks = SDL_GetGamepads(&count);

	if (count > 0) {

		m_SDLgamepad = SDL_OpenGamepad(joysticks[0]);
		if (m_SDLgamepad) {
			m_data.connected = true;
			m_data.name = SDL_GetGamepadName(m_SDLgamepad);
			m_data.type = SDL_GetGamepadTypeForID(joysticks[0]);
		}
	}
	SDL_free(joysticks);
}

void Gamepad::close()
{
	if (m_SDLgamepad) {
		SDL_CloseGamepad(m_SDLgamepad);
		m_SDLgamepad = nullptr;
		m_data.connected = false;
		m_data.name = "Unknown";
		m_data.type = SDL_GAMEPAD_TYPE_UNKNOWN;
	}
}

float Gamepad::normalizeAxis(Sint16 value)
{
	// MAYBE ADD A DEADZONE FUNCTION TOO...
	float normalized = (float)value / 32767.0f;
	return normalized;
}
