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

// - - - - - - - - - - - - - - - -

#include "TextureComponent.h"
#include "TextComponent.h"
#include "GameComponent.h"
#include "Player.h"
#include "EnemySpawner.h"
#include "Grid.h"
#include "Commands.h"
#include "GameSounds.h"

static void load()
{
	dae::Scene& scene{ dae::SceneManager::Instance().CreateScene() };

	std::shared_ptr<dae::Font> smoothFont{ dae::ResourceManager::Instance().LoadFont("Lingua.otf", 36) };
	std::shared_ptr<dae::Font> arcadeFont{ dae::ResourceManager::Instance().LoadFont("Arcade.TTF", 36) };

	dae::ServiceLocator::GetSoundSystem().LoadSoundMap(dae::gIdPathMap);

	{
		//background
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Background") };
		std::unique_ptr<dae::TextureComponent> renderComponent{ std::make_unique<dae::TextureComponent>(gameObject.get(), "background.png") };
		gameObject->AddComponent<dae::TextureComponent>(std::move(renderComponent));
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
		// grid object
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Grid") };

		std::unique_ptr<dae::EnemySpawner> spawnerComponent{ std::make_unique<dae::EnemySpawner>(gameObject.get()) };
		gameObject->AddComponent<dae::EnemySpawner>(std::move(spawnerComponent));

		std::unique_ptr<dae::GridComponent> gridComponent{ std::make_unique<dae::GridComponent>(gameObject.get()) };

		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_SCANCODE_F1, dae::KeyState::down },
			std::make_unique<dae::NextLevelCommand>(gridComponent.get())
		); //Skip level

		gameObject->AddComponent<dae::GridComponent>(std::move(gridComponent));

		scene.Add(std::move(gameObject));
	}

	{
		// player object
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Player") };
		gameObject->SetLocalPosition(scene.GetObjectWithName("Grid"_h)->GetComponent<dae::GridComponent>()->PlayerSpawn());

		std::unique_ptr<dae::PlayerComponent> playerComponent{ std::make_unique<dae::PlayerComponent>(gameObject.get()) };
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_SCANCODE_D, dae::KeyState::pressed },
			std::make_unique<dae::PlayerMoveCommand>(playerComponent.get(), dae::Direction::right)
		); //right
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_SCANCODE_A, dae::KeyState::pressed },
			std::make_unique<dae::PlayerMoveCommand>(playerComponent.get(), dae::Direction::left)
		); //left
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_SCANCODE_W, dae::KeyState::pressed },
			std::make_unique<dae::PlayerMoveCommand>(playerComponent.get(), dae::Direction::up)
		); //up
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_SCANCODE_S, dae::KeyState::pressed },
			std::make_unique<dae::PlayerMoveCommand>(playerComponent.get(), dae::Direction::down)
		); //down
		gameObject->AddComponent<dae::PlayerComponent>(std::move(playerComponent));

		std::unique_ptr<dae::TextureComponent> renderComponent{ std::make_unique<dae::TextureComponent>(gameObject.get(), "sprites/idle.png") };
		gameObject->AddComponent<dae::TextureComponent>(std::move(renderComponent));

		scene.Add(std::move(gameObject));
	}

	dae::InputManager::Instance().AddKeyBind(
		dae::KeyTrigger{ SDL_SCANCODE_F2, dae::KeyState::down },
		std::make_unique<dae::ToggleMuteCommand>()
	); // mute command
}

int main(int, char*[])
{
	std::filesystem::path data_location{ "./Data/" };
	if(!std::filesystem::exists(data_location))
		data_location = "../Data/";

	try
	{
		dae::Settings settings{
			.screenWidth = 1080,
			.screenHeight = 720,
			.gameWidth = 16*16,
			.gameHeight = 18*16,
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
