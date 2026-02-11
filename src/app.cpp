#include "app.h"
#include "inter.h"
#include "ui.h"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <iostream>

App::App(const std::string& title, int width, int height)
	: m_title(title), m_width(width), m_height(height) { }

App::~App()
{
	m_ui.cleanup();

	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

bool App::init()
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) return false;

	if (!SDL_CreateWindowAndRenderer(m_title.c_str(), m_width, m_height, 0, &m_window, &m_renderer))
	{
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplSDL3_InitForSDLRenderer(m_window, m_renderer);
	ImGui_ImplSDLRenderer3_Init(m_renderer);

	ImFontConfig font_cfg;
	font_cfg.FontDataOwnedByAtlas = false;
	ImFont* font = io.Fonts->AddFontFromMemoryTTF(
		(void*)inter,
		sizeof(inter),
		18.0f,
		&font_cfg
	);

	m_ui.applyModernTheme();

	if (!m_ui.loadTextures(m_renderer))
	{
		std::cerr << "Warning: Failed to load UI textures" << std::endl;
	}

	if (font == nullptr)
	{
		std::cerr << "Error to load font!" << std::endl;
	}

	m_running = true;
	return true;
}

void App::processEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		m_gamepad.handleEvent(event);
		ImGui_ImplSDL3_ProcessEvent(&event);

		if (event.type == SDL_EVENT_QUIT)
		{
			m_running = false;
		}

		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_F8)
			{
				m_showDemo = !m_showDemo;
			}
			if (event.key.key == SDLK_F3)
			{
				m_showDemo = !m_showDemo;
			}
		}
	}
}

void App::update()
{
	m_gamepad.update();
}

void App::render()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	m_ui.draw(m_showDemo, &m_gamepad);

	SDL_SetRenderDrawColor(m_renderer, 20, 20, 20, 255);
	SDL_RenderClear(m_renderer);

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);

	SDL_RenderPresent(m_renderer);
}

void App::run()
{
	while (m_running)
	{
		processEvents();
		update();
		render();
	}
}