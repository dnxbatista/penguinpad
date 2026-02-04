#include "gamepad.h"
#include <iostream>

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
	if (!m_gamepad) return;

	for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i) {
		m_data.buttons[i] = SDL_GetGamepadButton(m_gamepad, (SDL_GamepadButton)i);
	}

	m_data.leftStick[0] = normalizeAxis(SDL_GetGamepadAxis(m_gamepad, SDL_GAMEPAD_AXIS_LEFTX));
	m_data.leftStick[1] = normalizeAxis(SDL_GetGamepadAxis(m_gamepad, SDL_GAMEPAD_AXIS_LEFTY));
	m_data.rightStick[0] = normalizeAxis(SDL_GetGamepadAxis(m_gamepad, SDL_GAMEPAD_AXIS_RIGHTX));
	m_data.rightStick[1] = normalizeAxis(SDL_GetGamepadAxis(m_gamepad, SDL_GAMEPAD_AXIS_RIGHTY));

	m_data.leftTrigger = (float)SDL_GetGamepadAxis(m_gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 32767.0f;
	m_data.rightTrigger = (float)SDL_GetGamepadAxis(m_gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 32767.0f;
}

void Gamepad::handleEvent(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_GAMEPAD_ADDED) {
		if (!m_gamepad) openFirstAvailable();
	}
	else if (event.type == SDL_EVENT_GAMEPAD_REMOVED) {
		if (m_gamepad && event.gdevice.which == SDL_GetGamepadID(m_gamepad)) {
			close();
		}
	}
}

void Gamepad::openFirstAvailable()
{
	int count;
	SDL_JoystickID* joysticks = SDL_GetGamepads(&count);

	if (count > 0) {

		m_gamepad = SDL_OpenGamepad(joysticks[0]);
		if (m_gamepad) {
			m_data.connected = true;
			m_data.name = SDL_GetGamepadName(m_gamepad);
		}
	}
	SDL_free(joysticks);
}

void Gamepad::close()
{
	if (m_gamepad) {
		SDL_CloseGamepad(m_gamepad);
		m_gamepad = nullptr;
		m_data.connected = false;
		m_data.name = "Unknown";
	}
}

float Gamepad::normalizeAxis(Sint16 value)
{
	// MAYBE ADD A DEADZONE FUNCTION TOO...
	float normalized = (float)value / 32767.0f;
	return normalized;
}
