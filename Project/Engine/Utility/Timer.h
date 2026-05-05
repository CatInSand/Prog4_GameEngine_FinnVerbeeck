#ifndef TIMER_H
#define TIMER_H

namespace dae
{
	class Timer final
	{
	public:
		Timer(float duration);

		void Update();
		void Reset();

		float ElapsedTime() const;
		float Duration() const;
		bool Done() const;
		bool DoneThisFrame() const;

	private:
		const float m_Duration;
		float m_ElapsedTime{};
	};
}

#endif
