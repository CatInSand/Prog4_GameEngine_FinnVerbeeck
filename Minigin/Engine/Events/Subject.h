#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"

namespace dae
{
	class Subject final
	{
	public:
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
		void NotifyObservers(std::unique_ptr<Event>& pEvent)
		{
			for (Observer* pObserver : m_Observers)
			{
				pObserver->Notify(pEvent);
			}
		}

	private:
		std::vector<Observer*> m_Observers{};
	};
}

#endif