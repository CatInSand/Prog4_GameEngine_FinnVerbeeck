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

	//rotators
	std::unique_ptr<dae::GameObject> pivot{ std::make_unique<dae::GameObject>(root.get()) };
	pivot->SetLocalPosition(0.f, 0.f);

	gameObject = std::make_unique<dae::GameObject>(pivot.get());
	gameObject->SetLocalPosition(100.f, 0.f);
	renderComponent = std::make_unique<dae::RenderComponent>(gameObject.get());
	renderComponent->SetTexture("digdug.png");
	std::unique_ptr<dae::RotatorComponent> rotatorComponent{ std::make_unique<dae::RotatorComponent>(gameObject.get(), 1.f) };
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	gameObject->AddComponent<dae::RotatorComponent>(std::move(rotatorComponent));

	std::unique_ptr<dae::GameObject> childObject{ std::make_unique<dae::GameObject>(gameObject.get()) };
	childObject->SetLocalPosition(100.f, 0.f);
	renderComponent = std::make_unique<dae::RenderComponent>(childObject.get());
	renderComponent->SetTexture("digdug.png");
	rotatorComponent = std::make_unique<dae::RotatorComponent>(childObject.get(), -10.f);
	childObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	childObject->AddComponent<dae::RotatorComponent>(std::move(rotatorComponent));

	scene.Add(std::move(pivot));
	scene.Add(std::move(gameObject));
	scene.Add(std::move(childObject));

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
