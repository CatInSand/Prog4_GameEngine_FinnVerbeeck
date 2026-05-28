#ifndef TIMER_H
#define TIMER_H

namespace dae
{
	class Timer final
	{
	public:
		Timer(float duration);

		/*
		Increments elapsed time by dae::time::gDeltaTime
		*/
		void Update();
		/*
		Sets elapsed time to 0
		*/
		void Reset();

		/*
		Returns the number of times duration has elapsed and applies modulo to elapsed time using duration
		*/
		int ModuloGet();
		/*
		Returns true if done and decrements elapsed time by duration
		*/
		bool DecrementGet();

		float ElapsedTime() const;
		float Duration() const;

		bool Done() const;
		bool DoneThisFrame() const;

	private:
		float m_Duration; //non-const cause changes during assignment
		float m_ElapsedTime{ 0.f };
	};
}

#endif
