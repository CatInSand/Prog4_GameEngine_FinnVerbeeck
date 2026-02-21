#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace dae
{
	class Minigin final
	{
	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
<<<<<<< Updated upstream
=======
		void RunOneFrame();
		float GetDeltaTime();
>>>>>>> Stashed changes

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
<<<<<<< Updated upstream
=======

	private:
		bool m_quit{ false };
		uint64_t m_LastTime{ SDL_GetTicks() };
>>>>>>> Stashed changes
	};
}