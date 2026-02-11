#pragma once
#include "ui.h"
#include "gamepad.h"
#include <SDL3/SDL.h>
#include <string>

class App
{
public:
	App(const std::string& title, int width, int height);
	~App();
	bool init();
	void run();

private:
	void processEvents();
	void update();
	void render();

	// Window
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	bool m_running = false;
	std::string m_title;
	int m_width, m_height;

	// External References
	UI m_ui;
	Gamepad m_gamepad;

	// Debug
	bool m_showDemo = false;
};
