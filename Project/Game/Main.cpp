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
#include "OmniCommand.h"

// - - - - - - - - - - - - - - - -

#include "TextureComponent.h"
#include "TextComponent.h"
#include "GameComponent.h"
#include "ScoreComponent.h"
#include "Player.h"
#include "EnemySpawner.h"
#include "Grid.h"
#include "Commands.h"
#include "GameSounds.h"
#include "SoundComponent.h"

static void load()
{
	dae::Scene& scene{ dae::SceneManager::Instance().CreateScene() };

	std::shared_ptr<dae::Font> smoothFont{ dae::ResourceManager::Instance().LoadFont("Lingua.otf", 36) };
	std::shared_ptr<dae::Font> arcadeFont{ dae::ResourceManager::Instance().LoadFont("Arcade.TTF", 36) };

	dae::ServiceLocator::GetSoundSystem().LoadSoundMap(dae::gIdPathMap);

	{
		// game manager object
		std::unique_ptr<dae::GameObject> textObject{ std::make_unique<dae::GameObject>(scene.Root(), "ScoreText") };
		std::unique_ptr<dae::TextComponent> textComponent{ std::make_unique<dae::TextComponent>(textObject.get(), "SCORE: 0", arcadeFont) };

		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "GameManager") };
		std::unique_ptr<dae::GameComponent> gameComponent{ std::make_unique<dae::GameComponent>(gameObject.get()) };
		gameObject->AddComponent<dae::GameComponent>(std::move(gameComponent));
		std::unique_ptr<dae::SoundComponent> soundComponent{ std::make_unique<dae::SoundComponent>(gameObject.get()) };
		gameObject->AddComponent<dae::SoundComponent>(std::move(soundComponent));
		std::unique_ptr<dae::ScoreComponent> scoreComponent{ std::make_unique<dae::ScoreComponent>(gameObject.get(), textComponent.get()) };
		gameObject->AddComponent<dae::ScoreComponent>(std::move(scoreComponent));

		textObject->AddComponent<dae::TextComponent>(std::move(textComponent));
		textObject->SetParent(gameObject.get());
		scene.Add(std::move(textObject));
		scene.Add(std::move(gameObject));
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
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_SCANCODE_SPACE, dae::KeyState::down },
			std::make_unique<dae::PlayerShootCommand>(playerComponent.get())
		); //shoot

		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_GAMEPAD_BUTTON_DPAD_RIGHT, dae::KeyState::pressed },
			std::make_unique<dae::PlayerMoveCommand>(playerComponent.get(), dae::Direction::right)
		); //gamepad right
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_GAMEPAD_BUTTON_DPAD_LEFT, dae::KeyState::pressed },
			std::make_unique<dae::PlayerMoveCommand>(playerComponent.get(), dae::Direction::left)
		); //gamepad left
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_GAMEPAD_BUTTON_DPAD_UP, dae::KeyState::pressed },
			std::make_unique<dae::PlayerMoveCommand>(playerComponent.get(), dae::Direction::up)
		); //gamepad up
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_GAMEPAD_BUTTON_DPAD_DOWN, dae::KeyState::pressed },
			std::make_unique<dae::PlayerMoveCommand>(playerComponent.get(), dae::Direction::down)
		); //gamepad down
		dae::InputManager::Instance().AddKeyBind(
			dae::KeyTrigger{ SDL_GAMEPAD_BUTTON_SOUTH, dae::KeyState::down },
			std::make_unique<dae::PlayerShootCommand>(playerComponent.get())
		); //gamepad shoot
		gameObject->AddComponent<dae::PlayerComponent>(std::move(playerComponent));

		std::unique_ptr<dae::TextureComponent> renderComponent{ std::make_unique<dae::TextureComponent>(gameObject.get(), "sprites/idle.png") };
		gameObject->SetLocalScale(renderComponent->GetSize());
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

	//controls
	std::cout << "- - - - CONTROLS - - - -\n";
	std::cout << "Skip level: F1\n";
	std::cout << "Mute: F2\n";
	std::cout << "\n";
	std::cout << "- - - - GAMEPLAY - - - -\n";
	std::cout << "Movement: WASD or controller DPAD\n";
	std::cout << "Pump: Space or controller south button (A)\n";
	std::cout << "\n";

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
