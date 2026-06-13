#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Engine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "EventQueue.h"
#include "ServiceLocator.h"

#include <time.h>
#include <thread>
#include "DeltaTime.h"

SDL_Window* g_Window{};

void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
	// version = IMG_Version();
	// LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

dae::Engine::Engine(const std::filesystem::path& dataPath, Settings&& settings)
	: m_Settings{ std::move(settings) }
{
	PrintSDLVersion();
	
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		m_Settings.screenWidth,
		m_Settings.screenHeight,
		SDL_WINDOW_OPENGL
	);
	if (g_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	float scale{ std::min(
		static_cast<float>(m_Settings.screenWidth) / m_Settings.gameWidth,
		static_cast<float>(m_Settings.screenHeight) / m_Settings.gameHeight)
	};
	Renderer::Instance().Init(g_Window, scale);
	ResourceManager::Instance().Init(dataPath);

#ifndef NDEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SoundSystem>(m_Settings.masterVolume)));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SoundSystem>(m_Settings.masterVolume));
#endif

	EventQueue::Instance().AddObserver(&ServiceLocator::GetSoundSystem());
}

#include "CollisionQueue.h"

dae::Engine::~Engine()
{
	Renderer::Instance().Destroy();
	SDL_DestroyWindow(g_Window);
	g_Window = nullptr;

	ServiceLocator::RegisterSoundSystem(nullptr);
	SDL_Quit();

	SceneManager::Instance().Destroy();
}

void dae::Engine::Run(const std::function<void()>& load)
{
	//initialize
	load();

#ifndef __EMSCRIPTEN__
	while (!m_Quit) {
		dae::Engine::RunOneFrame();
	}
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void dae::Engine::RunOneFrame()
{
	time::CalculateDeltaTime();

	m_Quit = !InputManager::Instance().ProcessInput();
	SceneManager::Instance().Update();
	Renderer::Instance().Render();
}