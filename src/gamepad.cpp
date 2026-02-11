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

void Gamepad::setLightbar(uint8_t r, uint8_t g, uint8_t b)
{
	if (m_SDLgamepad) {
		SDL_SetGamepadLED(m_SDLgamepad, r, g, b);
	}
}

bool Gamepad::rumble(float lowFrequency, float highFrequency, uint32_t durationMs)
{
	if (!m_SDLgamepad) {
		return false;
	}

	auto clamp01 = [](float value) {
		if (value < 0.0f) return 0.0f;
		if (value > 1.0f) return 1.0f;
		return value;
	};

	Uint16 lowValue = static_cast<Uint16>(clamp01(lowFrequency) * 65535.0f);
	Uint16 highValue = static_cast<Uint16>(clamp01(highFrequency) * 65535.0f);

	return SDL_RumbleGamepad(m_SDLgamepad, lowValue, highValue, static_cast<Uint32>(durationMs));
}

bool Gamepad::rumbleTriggers(float left, float right, uint32_t durationMs)
{
	if (!m_SDLgamepad) {
		return false;
	}

	auto clamp01 = [](float value) {
		if (value < 0.0f) return 0.0f;
		if (value > 1.0f) return 1.0f;
		return value;
	};

	Uint16 leftValue = static_cast<Uint16>(clamp01(left) * 65535.0f);
	Uint16 rightValue = static_cast<Uint16>(clamp01(right) * 65535.0f);

	return SDL_RumbleGamepadTriggers(m_SDLgamepad, leftValue, rightValue, static_cast<Uint32>(durationMs));
}

bool Gamepad::hasGyro() const
{
	return m_SDLgamepad && SDL_GamepadHasSensor(m_SDLgamepad, SDL_SENSOR_GYRO);
}

bool Gamepad::setGyroEnabled(bool enabled)
{
	if (!m_SDLgamepad) {
		return false;
	}

	return SDL_SetGamepadSensorEnabled(m_SDLgamepad, SDL_SENSOR_GYRO, enabled);
}

bool Gamepad::gyroEnabled() const
{
	return m_SDLgamepad && SDL_GamepadSensorEnabled(m_SDLgamepad, SDL_SENSOR_GYRO);
}

bool Gamepad::getGyro(float out[3]) const
{
	if (!m_SDLgamepad) {
		return false;
	}

	return SDL_GetGamepadSensorData(m_SDLgamepad, SDL_SENSOR_GYRO, out, 3);
}

float Gamepad::gyroRate() const
{
	if (!m_SDLgamepad) {
		return 0.0f;
	}

	return SDL_GetGamepadSensorDataRate(m_SDLgamepad, SDL_SENSOR_GYRO);
}

int Gamepad::getNumTouchpads() const
{
	if (!m_SDLgamepad) return 0;
	return SDL_GetNumGamepadTouchpads(m_SDLgamepad);
}

int Gamepad::getNumTouchpadFingers(int touchpadIndex) const
{
	if (!m_SDLgamepad) return 0;
	return SDL_GetNumGamepadTouchpadFingers(m_SDLgamepad, touchpadIndex);
}

bool Gamepad::getTouchpadFinger(int touchpadIndex, int fingerIndex, float& x, float& y, float& pressure) const
{
	if (!m_SDLgamepad) return false;
	bool state;
	return SDL_GetGamepadTouchpadFinger(m_SDLgamepad, touchpadIndex, fingerIndex, &state, &x, &y, &pressure) && state;
}
