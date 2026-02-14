#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "FPSCounter.h"
#include "Scene.h"

#include <filesystem>

static void load()
{
	dae::Scene& scene{ dae::SceneManager::GetInstance().CreateScene() };

	std::unique_ptr<dae::GameObject> gameObject{ std::make_unique<dae::GameObject>() };
	gameObject->SetTexture("background.png");
	scene.Add(std::move(gameObject));

	gameObject = std::make_unique<dae::GameObject>();
	gameObject->SetTexture("logo.png");
	gameObject->SetPosition(358, 180);
	scene.Add(std::move(gameObject));

	std::shared_ptr<dae::Font> font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };

	std::unique_ptr<dae::TextObject> textObject{ std::make_unique<dae::TextObject>("Programming 4 Assignment", font) };
	textObject->SetColor({ 255, 255, 0, 255 });
	textObject->SetPosition(292, 20);

	std::unique_ptr<dae::GameObject> textObject2{ std::make_unique<dae::GameObject>() };
	std::unique_ptr<dae::FPSCounter> fpsComponent{ std::make_unique<dae::FPSCounter>(textObject2, "FPS", font)};
	fpsComponent->SetColor({ 255, 255, 0, 255 });
	textObject2->AddComponent(std::move(fpsComponent));
	textObject2->SetPosition(292, 20);

	scene.Add(std::move(textObject));
	scene.Add(std::move(textObject2));
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
