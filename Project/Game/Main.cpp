#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Engine.h"
#include "Settings.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ResourceManager.h"

#include <filesystem>
#include <iostream>
#include "InputManager.h"
#include "MoveCommand.h"

#if !__EMSCRIPTEN__
#include <windows.h>
#include <Xinput.h>
#else
//emscripten includes
#endif

// - - - - - - - - - - - - - - - -

#include "TextureComponent.h"
#include "TextComponent.h"
#include "GameComponent.h"
#include "Player.h"

static void load()
{
	dae::Scene& scene{ dae::SceneManager::Instance().CreateScene() };

	std::shared_ptr<dae::Font> smoothFont{ dae::ResourceManager::Instance().LoadFont("Lingua.otf", 36) };
	std::shared_ptr<dae::Font> arcadeFont{ dae::ResourceManager::Instance().LoadFont("Arcade.TTF", 36) };

	{
		//background
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Background") };
		std::unique_ptr<dae::TextureComponent> renderComponent{ std::make_unique<dae::TextureComponent>(gameObject.get(), "background.png") };
		gameObject->AddComponent<dae::TextureComponent>(std::move(renderComponent));
		scene.Add(std::move(gameObject));
	}

	{
		//text
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Text") };
		std::unique_ptr<dae::TextComponent> textComponent{ std::make_unique<dae::TextComponent>(gameObject.get(), "no gameplay currently :(", smoothFont) };
		gameObject->AddComponent<dae::TextComponent>(std::move(textComponent));
		scene.Add(std::move(gameObject));
	}

	{
		// game manager object
		std::unique_ptr<dae::GameObject> textObject{ std::make_unique<dae::GameObject>(scene.Root(), "ScoreText") };
		std::unique_ptr<dae::TextComponent> textComponent{ std::make_unique<dae::TextComponent>(textObject.get(), "SCORE: 0", arcadeFont) };

		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "GameManager") };
		std::unique_ptr<dae::GameComponent> gameComponent{ std::make_unique<dae::GameComponent>(gameObject.get(), textComponent.get()) };
		gameObject->AddComponent<dae::GameComponent>(std::move(gameComponent));

		textObject->AddComponent<dae::GameComponent>(std::move(textComponent));
		textObject->SetParent(gameObject.get());
		scene.Add(std::move(gameObject));
		scene.Add(std::move(textObject));
	}
	
	{
		// player object
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Player") };
		std::unique_ptr<dae::PlayerComponent> playerComponent{ std::make_unique<dae::PlayerComponent>(gameObject.get())};
		gameObject->AddComponent<dae::PlayerComponent>(std::move(playerComponent));
		scene.Add(std::move(gameObject));
	}
}

int main(int, char*[])
{
#if __EMSCRIPTEN__
	std::filesystem::path data_location{ "" };
#else
	std::filesystem::path data_location{ "./Data/" };
	if(!std::filesystem::exists(data_location))
		data_location = "../Data/";
#endif

	try
	{
		dae::Settings settings{
			.screenWidth = 1080,
			.screenHeight = 720,
			.gameWidth = 224,
			.gameHeight = 288,
			.masterVolume = 0.2f,
		};

		dae::Engine engine(data_location, std::move(settings));
		engine.Run(load);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception thrown: " << e.what() << "\n";
	}

    return 0;
}
