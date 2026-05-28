#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>

#include "Observer.h"

namespace dae
{
	class _BaseSubject
	{
	public:
		_BaseSubject() = default;
		virtual ~_BaseSubject() = default;

		void AddObserver(Observer* pObserver)
		{
			m_Observers.push_back(pObserver);
		}
		void RemoveObserver(Observer* pObserver)
		{
			m_Observers.erase(
				std::remove(m_Observers.begin(), m_Observers.end(), pObserver),
				m_Observers.end()
			);
		}

	protected:
		void _NotifyObservers(std::unique_ptr<Event>& pEvent)
		{
			for (Observer* pObserver : m_Observers)
			{
				pObserver->Notify(pEvent);
			}
		}

		std::vector<Observer*> m_Observers{};
	};

	class Subject final : public _BaseSubject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;

		void NotifyObservers(std::unique_ptr<Event>& pEvent)
		{
			_NotifyObservers(pEvent);
		}
	};
}

#endif