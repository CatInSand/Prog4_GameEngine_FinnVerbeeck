#include "Observer.h"
#include "Text.h"

void dae::DeathObserver::Notify(std::unique_ptr<Event>& pEvent)
{
	if (pEvent->m_ID == hash::make_sdbm_hash("PlayerDied"))
	{
		EventPlayerDied* currentEvent{ reinterpret_cast<EventPlayerDied*>(pEvent.get()) };
		currentEvent->m_pTarget->GetComponent<dae::Text>()->SetText("Dead");
	}
}