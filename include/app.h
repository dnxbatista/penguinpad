#pragma once
#include "ui.h"
#include "controller.h"
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
	void applyModernTheme();
	void processEvents();
	void update();
	void render();

	// Window
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	bool m_running = false;
	std::string m_title;
	int m_width, m_heigth;

	// External References
	UI m_ui;
	Controller m_controller;

	// Debug
	bool m_showDemo = false;
};
