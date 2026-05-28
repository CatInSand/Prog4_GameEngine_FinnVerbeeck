#include "DeathObserver.h"
#include "GameEvents.h"
#include "TextComponent.h"

void dae::DeathObserver::Notify(std::unique_ptr<Event>& pEvent)
{
	if (pEvent->m_ID == "PlayerDied"_h)
	{
		EventPlayerDied* currentEvent{ reinterpret_cast<EventPlayerDied*>(pEvent.get()) };
		currentEvent->m_pTarget->GetComponent<dae::TextComponent>()->SetText("Dead");
	}
}