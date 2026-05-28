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
		float m_Duration; //non-const cause changes during assignment
		float m_ElapsedTime{ 0.f };
	};
}

#endif
