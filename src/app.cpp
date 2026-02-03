#include "app.h"
#include "inter.h"
#include "ui.h"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <iostream>

App::App(const std::string& title, int width, int height)
	: m_title(title), m_width(width), m_heigth(height) { }

App::~App()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void App::applyModernTheme() {
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

bool App::init()
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) return false;

	if (!SDL_CreateWindowAndRenderer(m_title.c_str(), m_width, m_heigth, 0, &m_window, &m_renderer))
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

	applyModernTheme();

	if (font == nullptr)
	{
		std::cerr << "Error to load font!" << std::endl;
	}

	static const ImWchar ranges[] = { // Basic latin support
		0x0020, 0x00FF, 0
	};

	m_running = true;
	return true;
}

void App::processEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		m_controller.handleEvent(event);
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
		}
	}
}

void App::update()
{
	m_controller.update();
}

void App::render()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	m_ui.draw(m_showDemo, &m_controller);

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