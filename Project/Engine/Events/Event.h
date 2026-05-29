#ifndef EVENT_H
#define EVENT_H

#include "GameObject.h"
#include <cat/hash.h>

using namespace cat::hash_literals; //add hash literal overload "string"_h

namespace dae
{
	using EventID = cat::hash_t;

	class Event
	{
	public:
		virtual ~Event() = default;
		const EventID m_ID;

	protected:
		explicit Event(EventID id)
			: m_ID{ id }
		{};
	};
}

#endif