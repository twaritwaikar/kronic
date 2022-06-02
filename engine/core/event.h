#pragma once

#include "core/math.h"
#include "event_link.h"

template <class EventData>
class BaseEvent
{
public:
	static inline Vector<BaseEventLink<EventData>*> queue;
	static void propagate(const EventData& e)
	{
		for (BaseEventLink<EventData>* event_link : queue)
		{
			event_link->fire(e);
		}
	}
};

struct Event
{
	struct Window
	{
		struct Resized : public BaseEvent<Resized>
		{
			uint32_t width;
			uint32_t height;
		};
	};
};
