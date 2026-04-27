#ifndef EVENT_H
#define EVENT_H

#include "GameObject.h"

namespace dae
{
	namespace hash
	{
		template <int length>
		struct sdbm_hash
		{
			consteval static unsigned int _calculate(const char* const text, unsigned int& value)
			{
				const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
				value = character + (value << 6) + (value << 16) - value;
				return text[length - 1];
			}
			consteval static unsigned int calculate(const char* const text)
			{
				unsigned int value = 0;
				const auto character = _calculate(text, value);
				return character + (value << 6) + (value << 16) - value;
			}
		};
		template <>
		struct sdbm_hash<1>
		{
			consteval static int _calculate(const char* const text, unsigned int&) { return text[0]; }
		};
		template <size_t N>
		consteval unsigned int make_sdbm_hash(const char(&text)[N])
		{
			return sdbm_hash<N - 1>::calculate(text);
		}
	}

	using EventID = unsigned int;

	class Event
	{
	public:
		Event(EventID id)
			: m_ID{ id }
		{
		};
		const EventID m_ID;
	};
	
	class EventPlayerDied final : public Event
	{
	public:
		EventPlayerDied(GameObject* pTarget)
			: Event( hash::make_sdbm_hash("PlayerDied") )
			, m_pTarget{ pTarget }
		{
		}
		GameObject* m_pTarget;
	};
}

#endif