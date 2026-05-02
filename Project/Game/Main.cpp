#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "FPSCounter.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "Scene.h"

#include <filesystem>
#include "InputManager.h"
#include "MoveCommand.h"

#include "HealthComponent.h"
#include "DamageCommand.h"
#include "DeathObserverComponent.h"

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

	//background
	std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(scene.Root(), "Background") };
	std::unique_ptr<dae::RenderComponent> renderComponent{ std::make_unique<dae::RenderComponent>(gameObject.get()) };
	renderComponent->SetTexture("background.png");
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	scene.Add(std::move(gameObject));

	gameObject = std::make_unique<dae::GameObject>(scene.Root(), "Logo");
	renderComponent = std::make_unique<dae::RenderComponent>(gameObject.get());
	renderComponent->SetTexture("logo.png");
	gameObject->SetLocalPosition(358, 180);
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	scene.Add(std::move(gameObject));

	std::shared_ptr<dae::Font> font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };

	gameObject = std::make_unique<dae::GameObject>(scene.Root(), "Text");
	std::unique_ptr<dae::TextComponent> textComponent{ std::make_unique<dae::TextComponent>(gameObject.get(), "Programming 4 Assignment", font) };
	textComponent->SetColor({ 255, 255, 0, 255 });
	gameObject->SetLocalPosition(292, 20);
	gameObject->AddComponent<dae::TextComponent>(std::move(textComponent));
	scene.Add(std::move(gameObject));

	//fps
	gameObject = std::make_unique<dae::GameObject>(scene.Root(), "FPS counter");
	textComponent = std::make_unique<dae::TextComponent>(gameObject.get(), "FPS", font);
	std::unique_ptr<dae::FPSCounter> fpsCounterComponent{ std::make_unique<dae::FPSCounter>(gameObject.get()) };
	textComponent->SetColor({ 255, 255, 255, 255 });
	gameObject->SetLocalPosition(0, 0);
	gameObject->AddComponent<dae::FPSCounter>(std::move(fpsCounterComponent));
	gameObject->AddComponent<dae::TextComponent>(std::move(textComponent));
	std::unique_ptr<dae::FPSComponent> fpsComponent{ std::make_unique<dae::FPSComponent>(gameObject.get()) };
	gameObject->AddComponent<dae::FPSComponent>(std::move(fpsComponent));
	scene.Add(std::move(gameObject));

	//keybinds
	gameObject = std::make_unique<dae::GameObject>(scene.Root(), "Player1");
	gameObject->SetLocalPosition(100.f, 0.f);

	renderComponent = std::make_unique<dae::RenderComponent>(gameObject.get());
	renderComponent->SetTexture("digdug.png");
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));

	std::unique_ptr<dae::DeathObserverComponent> deathObserver{ std::make_unique<dae::DeathObserverComponent>(gameObject.get()) };
	std::unique_ptr<dae::HealthComponent> healthComponent{ std::make_unique<dae::HealthComponent>(gameObject.get(), 3, deathObserver->Get()) };
	std::unique_ptr<dae::DamageCommand> damageCommand{ std::make_unique<dae::DamageCommand>(healthComponent.get()) };
	dae::KeyTrigger keyTriggerXPressed{ SDL_SCANCODE_X, dae::KeyState::down };
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerXPressed, std::move(damageCommand));
	gameObject->AddComponent<dae::HealthComponent>(std::move(healthComponent));
	gameObject->AddComponent<dae::DeathObserverComponent>(std::move(deathObserver));

	textComponent = std::make_unique<dae::TextComponent>(gameObject.get(), "Alive", font);
	textComponent->SetColor({ 255, 255, 255, 255 });
	gameObject->AddComponent<dae::TextComponent>(std::move(textComponent));

	constexpr float SPEED{ 100.f };
	std::unique_ptr<dae::MoveCommand> moveCommandUp{ std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{0.f, -1.f}, SPEED) };
	std::unique_ptr<dae::MoveCommand> moveCommandDown{ std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{0.f, 1.f}, SPEED) };
	std::unique_ptr<dae::MoveCommand> moveCommandLeft{ std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{-1.f, 0.f}, SPEED) };
	std::unique_ptr<dae::MoveCommand> moveCommandRight{ std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{1.f, 0.f}, SPEED) };

	dae::KeyTrigger keyTriggerWPressed{ SDL_SCANCODE_W, dae::KeyState::pressed };
	dae::KeyTrigger keyTriggerSPressed{ SDL_SCANCODE_S, dae::KeyState::pressed };
	dae::KeyTrigger keyTriggerAPressed{ SDL_SCANCODE_A, dae::KeyState::pressed };
	dae::KeyTrigger keyTriggerDPressed{ SDL_SCANCODE_D, dae::KeyState::pressed };
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerWPressed, std::move(moveCommandUp));
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerSPressed, std::move(moveCommandDown));
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerAPressed, std::move(moveCommandLeft));
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerDPressed, std::move(moveCommandRight));

	scene.Add(std::move(gameObject));

	//controller bindings
	gameObject = std::make_unique<dae::GameObject>(scene.Root(), "Player2");
	gameObject->SetLocalPosition(200.f, 0.f);
	renderComponent = std::make_unique<dae::RenderComponent>(gameObject.get());
	renderComponent->SetTexture("digdug.png");
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));

	moveCommandUp = std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{0.f, -1.f}, SPEED * 2.f);
	moveCommandDown = std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{0.f, 1.f}, SPEED * 2.f);
	moveCommandLeft = std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{-1.f, 0.f}, SPEED * 2.f);
	moveCommandRight = std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{1.f, 0.f}, SPEED * 2.f);

#if !__EMSCRIPTEN__
	dae::KeyTrigger padTriggerUpPressed{ XINPUT_GAMEPAD_DPAD_UP, dae::KeyState::pressed };
	dae::KeyTrigger padTriggerDownPressed{ XINPUT_GAMEPAD_DPAD_DOWN, dae::KeyState::pressed };
	dae::KeyTrigger padTriggerLeftPressed{ XINPUT_GAMEPAD_DPAD_LEFT, dae::KeyState::pressed };
	dae::KeyTrigger padTriggerRightPressed{ XINPUT_GAMEPAD_DPAD_RIGHT, dae::KeyState::pressed };
#else
	dae::KeyTrigger padTriggerUpPressed{ SDL_GAMEPAD_BUTTON_DPAD_UP, dae::KeyState::pressed };
	dae::KeyTrigger padTriggerDownPressed{ SDL_GAMEPAD_BUTTON_DPAD_DOWN, dae::KeyState::pressed };
	dae::KeyTrigger padTriggerLeftPressed{ SDL_GAMEPAD_BUTTON_DPAD_LEFT, dae::KeyState::pressed };
	dae::KeyTrigger padTriggerRightPressed{ SDL_GAMEPAD_BUTTON_DPAD_RIGHT, dae::KeyState::pressed };
#endif
	dae::InputManager::GetInstance().AddKeyBind(padTriggerUpPressed, std::move(moveCommandUp));
	dae::InputManager::GetInstance().AddKeyBind(padTriggerDownPressed, std::move(moveCommandDown));
	dae::InputManager::GetInstance().AddKeyBind(padTriggerLeftPressed, std::move(moveCommandLeft));
	dae::InputManager::GetInstance().AddKeyBind(padTriggerRightPressed, std::move(moveCommandRight));

	scene.Add(std::move(gameObject));

	//sound
	gameObject = std::make_unique<dae::GameObject>(scene.Root(), "Text");
	textComponent = std::make_unique<dae::TextComponent>(gameObject.get(), "Press G to play sound :D", font);
	textComponent->SetColor({ 255, 255, 255, 255 });
	gameObject->SetLocalPosition(20, 520);
	gameObject->AddComponent<dae::TextComponent>(std::move(textComponent));
	scene.Add(std::move(gameObject));

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
