#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "MultiSpriteComponent.h"
#include "GridComponent.h"
#include "StateMachine.h"
#include "Settings.h"
#include "Pooka.h"
#include "Scene.h"
#include "ComponentList.h"

#include <filesystem>
#include "InputManager.h"
#include "MoveCommand.h"

#include "SoundCommand.h"

#if !__EMSCRIPTEN__
#include <windows.h>
#include <Xinput.h>
#else
//emscripten includes
#endif

enum class PlayerSprite : dae::sprite_id {
	idle,
	walking,
	drilling,
};


static void load()
{
	dae::Scene& scene{ dae::SceneManager::GetInstance().CreateScene() };

	std::shared_ptr<dae::Font> smoothFont{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	std::shared_ptr<dae::Font> arcadeFont{ dae::ResourceManager::GetInstance().LoadFont("Arcade.TTF", 36) };

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
		//grid
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Grid") };
		gameObject->SetLocalPosition(150.f, 0.f);

		std::unique_ptr<dae::RenderComponentList<dae::TextureComponent>> textureComponentList{
			std::make_unique<dae::RenderComponentList<dae::TextureComponent>>(gameObject.get())
		};
		gameObject->AddComponent<dae::RenderComponentList<dae::TextureComponent>>(std::move(textureComponentList));

		std::unique_ptr<dae::GridComponent> gridComponent{ std::make_unique<dae::GridComponent>(gameObject.get()) };
		gameObject->AddComponent<dae::GridComponent>(std::move(gridComponent));
		scene.Add(std::move(gameObject));
	}

	{
		//pooka
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Pooka") };
		gameObject->SetLocalPosition(100.f, 30.f);

		std::unordered_map<dae::sprite_id, dae::Sprite> spriteMap{
			{ static_cast<dae::sprite_id>(dae::Pooka::Sprites::walk), {"sprites/pooka_walk.png", dae::Sprite::Type::loop, 0.2f} },
			{ static_cast<dae::sprite_id>(dae::Pooka::Sprites::ghost), {"sprites/pooka_ghost.png", dae::Sprite::Type::loop, 0.2f} },
			{ static_cast<dae::sprite_id>(dae::Pooka::Sprites::flat), {"sprites/pooka_flat.png", dae::Sprite::Type::still} },
			{ static_cast<dae::sprite_id>(dae::Pooka::Sprites::blow), {"sprites/pooka_blow.png", dae::Sprite::Type::single} },
		};

		std::unique_ptr<dae::MultiSpriteComponent> spriteComponent{
			std::make_unique<dae::MultiSpriteComponent>(gameObject.get(), std::move(spriteMap), 0)
		};
		gameObject->AddComponent<dae::MultiSpriteComponent>(std::move(spriteComponent));

		std::unique_ptr<dae::Pooka::StateData> pookaStateData{
			std::make_unique<dae::Pooka::StateData>(gameObject.get())
		};
		std::vector<std::unique_ptr<dae::Pooka::State>> states{};
		states.push_back(std::make_unique<dae::Pooka::Idle>(pookaStateData.get()));
		states.push_back(std::make_unique<dae::Pooka::Ghost>(pookaStateData.get()));
		states.push_back(std::make_unique<dae::Pooka::Chase>(pookaStateData.get()));
		states.push_back(std::make_unique<dae::Pooka::Flat>(pookaStateData.get()));
		states.push_back(std::make_unique<dae::Pooka::Blow>(pookaStateData.get()));

		std::unique_ptr<dae::Pooka::StateMachine> stateMachine{
			std::make_unique<dae::Pooka::StateMachine>(gameObject.get(), std::move(states), std::move(pookaStateData), 0)
		};

		gameObject->AddComponent<dae::Pooka::StateMachine>(std::move(stateMachine));
		scene.Add(std::move(gameObject));
	}

	{
		//sprites
		std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Player1") };
		gameObject->SetLocalPosition(100.f, 0.f);

		std::unordered_map<dae::sprite_id, dae::Sprite> spriteMap{
			{ static_cast<dae::sprite_id>(PlayerSprite::idle), {"sprites/idle.png", dae::Sprite::Type::still} },
			{ static_cast<dae::sprite_id>(PlayerSprite::walking), {"sprites/walk.png", dae::Sprite::Type::loop, 0.2f} },
			{ static_cast<dae::sprite_id>(PlayerSprite::drilling), {"sprites/swing.png", dae::Sprite::Type::swing, 0.2f} },
		};

		std::unique_ptr<dae::MultiSpriteComponent> spriteComponent{
			std::make_unique<dae::MultiSpriteComponent>(gameObject.get(), std::move(spriteMap), 1)
		};
		gameObject->AddComponent<dae::MultiSpriteComponent>(std::move(spriteComponent));
		scene.Add(std::move(gameObject));
	}

	{
		//sound
		dae::KeyTrigger keyTriggerGDown{ SDL_SCANCODE_G, dae::KeyState::down };
		std::unique_ptr<dae::SoundCommand> soundCommand{ std::make_unique<dae::SoundCommand>(0, 1.f) };
		dae::InputManager::GetInstance().AddKeyBind(keyTriggerGDown, std::move(soundCommand));
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

	dae::Settings settings{
		.screenWidth = 1080,
		.screenHeight = 720,
		.gameWidth = 224,
		.gameHeight = 288,
	};

	dae::Minigin engine(data_location, std::move(settings));
	engine.Run(load);

    return 0;
}
