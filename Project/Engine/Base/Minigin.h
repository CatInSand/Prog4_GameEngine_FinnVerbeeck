#pragma once
#include <string>
#include <functional>
#include <filesystem>

#include "Settings.h"

namespace dae
{
	class Minigin final
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath, Settings&& settings);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();
		float GetDeltaTime();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		Settings m_Settings;
		bool m_quit{ false };
		uint64_t m_LastTime{ SDL_GetTicks() };
	};
}