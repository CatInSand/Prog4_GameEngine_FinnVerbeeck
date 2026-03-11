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

static void load()
{
	dae::Scene& scene{ dae::SceneManager::GetInstance().CreateScene() };

	std::unique_ptr<dae::GameObject> root{ std::make_unique<dae::GameObject>(nullptr) };

	//background
	std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>(root.get()) };
	std::unique_ptr<dae::RenderComponent> renderComponent{ std::make_unique<dae::RenderComponent>(gameObject.get()) };
	renderComponent->SetTexture("background.png");
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	scene.Add(std::move(gameObject));

	gameObject = std::make_unique<dae::GameObject>(root.get());
	renderComponent = std::make_unique<dae::RenderComponent>(gameObject.get());
	renderComponent->SetTexture("logo.png");
	gameObject->SetLocalPosition(358, 180);
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	scene.Add(std::move(gameObject));

	std::shared_ptr<dae::Font> font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };

	gameObject = std::make_unique<dae::GameObject>(root.get());
	std::unique_ptr<dae::Text> textComponent{ std::make_unique<dae::Text>(gameObject.get(), "Programming 4 Assignment", font) };
	textComponent->SetColor({ 255, 255, 0, 255 });
	gameObject->SetLocalPosition(292, 20);
	gameObject->AddComponent<dae::Text>(std::move(textComponent));
	scene.Add(std::move(gameObject));

	//fps
	gameObject = std::make_unique<dae::GameObject>(root.get());
	textComponent = std::make_unique<dae::Text>(gameObject.get(), "FPS", font);
	std::unique_ptr<dae::FPSCounter> fpsCounterComponent{ std::make_unique<dae::FPSCounter>(gameObject.get()) };
	textComponent->SetColor({ 255, 255, 255, 255 });
	gameObject->SetLocalPosition(0, 0);
	gameObject->AddComponent<dae::FPSCounter>(std::move(fpsCounterComponent));
	gameObject->AddComponent<dae::Text>(std::move(textComponent));
	std::unique_ptr<dae::FPSComponent> fpsComponent{ std::make_unique<dae::FPSComponent>(gameObject.get()) };
	gameObject->AddComponent<dae::FPSComponent>(std::move(fpsComponent));
	scene.Add(std::move(gameObject));

	//keybinds
	gameObject = std::make_unique<dae::GameObject>(root.get());
	gameObject->SetLocalPosition(100.f, 0.f);
	renderComponent = std::make_unique<dae::RenderComponent>(gameObject.get());
	renderComponent->SetTexture("digdug.png");
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	std::unique_ptr<dae::MoveCommand> moveCommandUp{ std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{0.f, -1.f}) };
	std::unique_ptr<dae::MoveCommand> moveCommandDown{ std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{0.f, 1.f}) };
	std::unique_ptr<dae::MoveCommand> moveCommandLeft{ std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{-1.f, 0.f}) };
	std::unique_ptr<dae::MoveCommand> moveCommandRight{ std::make_unique<dae::MoveCommand>(gameObject.get(), glm::vec2{1.f, 0.f}) };

	dae::KeyTrigger keyTriggerWDown{ SDL_SCANCODE_W, dae::KeyState::pressed };
	dae::KeyTrigger keyTriggerSDown{ SDL_SCANCODE_S, dae::KeyState::pressed };
	dae::KeyTrigger keyTriggerADown{ SDL_SCANCODE_A, dae::KeyState::pressed };
	dae::KeyTrigger keyTriggerDDown{ SDL_SCANCODE_D, dae::KeyState::pressed };
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerWDown, std::move(moveCommandUp));
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerSDown, std::move(moveCommandDown));
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerADown, std::move(moveCommandLeft));
	dae::InputManager::GetInstance().AddKeyBind(keyTriggerDDown, std::move(moveCommandRight));

	scene.Add(std::move(gameObject));

	//end
	scene.Add(std::move(root));
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
