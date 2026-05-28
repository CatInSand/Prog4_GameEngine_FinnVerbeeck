#pragma once
#include <string>
#include <functional>
#include <filesystem>

#include "Settings.h"

namespace dae
{
	class Engine final
	{
	public:
		explicit Engine(const std::filesystem::path& dataPath, Settings&& settings);
		~Engine();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;
		Engine& operator=(const Engine& other) = delete;
		Engine& operator=(Engine&& other) = delete;

	private:
		Settings m_Settings;
		bool m_Quit{ false };
	};
}