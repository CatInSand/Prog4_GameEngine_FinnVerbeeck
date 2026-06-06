#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "Settings.h"
#include "Scene.h"
#include "MultiSpriteComponent.h"

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
