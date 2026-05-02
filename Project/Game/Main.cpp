#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "SpriteComponent.h"
#include "Scene.h"

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

static void load()
{
	dae::Scene& scene{ dae::SceneManager::GetInstance().CreateScene() };

	std::shared_ptr<dae::Font> smoothFont{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	std::shared_ptr<dae::Font> arcadeFont{ dae::ResourceManager::GetInstance().LoadFont("Arcade.TTF", 36) };

	//background
	std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Background") };
	std::unique_ptr<dae::TextureComponent> renderComponent{ std::make_unique<dae::TextureComponent>(gameObject.get(), "background.png") };
	gameObject->AddComponent<dae::TextureComponent>(std::move(renderComponent));
	scene.Add(std::move(gameObject));

	//sprites
	gameObject = std::make_unique<dae::GameObject>(scene.Root(), "Player1");
	gameObject->SetLocalPosition(100.f, 0.f);

	dae::Sprite sprite{ "sprites/walk.png", dae::Sprite::Type::loop, 0.2f };

	std::unique_ptr<dae::SpriteComponent> spriteComponent{
		std::make_unique<dae::SpriteComponent>(gameObject.get(), std::move(sprite))
	};
	gameObject->AddComponent<dae::SpriteComponent>(std::move(spriteComponent));
	scene.Add(std::move(gameObject));

	//sound
	dae::KeyTrigger keyTriggerGDown{ SDL_SCANCODE_G, dae::KeyState::down };
	std::unique_ptr<dae::SoundCommand> soundCommand{ std::make_unique<dae::SoundCommand>(0, 1.f) };
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerGDown, std::move(soundCommand));
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

	dae::Minigin engine(data_location);
	engine.Run(load);

    return 0;
}
