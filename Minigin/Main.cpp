#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "FPSCounter.h"
#include "Scene.h"

#include <filesystem>

static void load()
{
	dae::Scene& scene{ dae::SceneManager::GetInstance().CreateScene() };

	std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>() };
	std::unique_ptr<dae::RenderComponent> renderComponent{ std::make_unique<dae::RenderComponent>(gameObject.get()) };
	renderComponent->SetTexture("background.png");
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	scene.Add(std::move(gameObject));

	gameObject = std::make_unique<dae::GameObject>();
	renderComponent = std::make_unique<dae::RenderComponent>(gameObject.get());
	renderComponent->SetTexture("logo.png");
	gameObject->SetPosition(358, 180);
	gameObject->AddComponent<dae::RenderComponent>(std::move(renderComponent));
	scene.Add(std::move(gameObject));

	std::shared_ptr<dae::Font> font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };

	gameObject = std::make_unique<dae::GameObject>();
	std::unique_ptr<dae::Text> textComponent{ std::make_unique<dae::Text>(gameObject.get(), "Programming 4 Assignment", font) };
	textComponent->SetColor({ 255, 255, 0, 255 });
<<<<<<< Updated upstream
	textComponent->SetPosition(292, 20);
	textObject->AddComponent(std::move(textComponent));
=======
	gameObject->SetPosition(292, 20);
	gameObject->AddComponent<dae::Text>(std::move(textComponent));
	scene.Add(std::move(gameObject));
>>>>>>> Stashed changes

	gameObject = std::make_unique<dae::GameObject>();
	std::unique_ptr<dae::FPSCounter> fpsComponent{ std::make_unique<dae::FPSCounter>(gameObject.get(), "FPS", font)};
	fpsComponent->SetColor({ 255, 255, 255, 255 });
<<<<<<< Updated upstream
	fpsComponent->SetPosition(0, 0);
	fpsObject->AddComponent(std::move(fpsComponent));

	scene.Add(std::move(textObject));
	scene.Add(std::move(fpsObject));
=======
	gameObject->SetPosition(0, 0);
	gameObject->AddComponent<dae::FPSCounter>(std::move(fpsComponent));
	scene.Add(std::move(gameObject));
>>>>>>> Stashed changes
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
