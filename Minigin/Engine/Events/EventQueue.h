#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Observer.h"
#include <vector>
#include <queue>

namespace dae
{
	class EventQueue final
	{
	public:
		EventQueue() = default;
		~EventQueue() = default;

		/**
		Immediate dispatch. Should not be used for general events.
		*/
		void Send(std::unique_ptr<Event>& pEvent)
		{
			for (Observer* pObserver : m_ObserverPointers)
			{
				pObserver->Notify(m_EventQueue.front());
			}
		}
		void Enqueue(std::unique_ptr<Event>&& pEvent)
		{
			m_EventQueue.push(pEvent);
		}
		void SendAll()
		{
			while(!m_EventQueue.empty())
			{
				Send(m_EventQueue.front());
				m_EventQueue.pop()
			}
		}

		void AddObserver(Observer* pObserver)
		{
			m_ObserverPointers.push_back(pObserver);
		}
		void RemoveObserver(Observer* pObserver)
		{
			m_ObserverPointers.erase(
				std::remove(m_ObserverPointers.begin(), m_ObserverPointers.end(), pObserver),
				m_ObserverPointers.end()
			);
		}

	private:
		std::vector<Observer*> m_ObserverPointers{};
		std::queue<std::unique_ptr<Event>> m_EventQueue{};
	};
}

#endif
