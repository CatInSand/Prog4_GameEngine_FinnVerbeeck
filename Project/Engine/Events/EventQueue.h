#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <vector>
#include <queue>
#include <Singleton.h>

#include "Subject.h"

namespace dae
{
	class EventQueue : public _BaseSubject, public Singleton<EventQueue>
	{
	public:
		EventQueue() = default;
		virtual ~EventQueue() = default;

		void BypassSend(std::unique_ptr<Event>& pEvent)
		{
			_NotifyObservers(pEvent);
		}
		void Enqueue(std::unique_ptr<Event>&& pEvent)
		{
			m_EventQueue.push(std::move(pEvent));
		}
		void SendAll()
		{
			while(!m_EventQueue.empty())
			{
				_NotifyObservers(m_EventQueue.front());
				m_EventQueue.pop();
			}
		}

	private:
		friend class Singleton<EventQueue>;

		std::queue<std::unique_ptr<Event>> m_EventQueue{};
	};
}

#endif
