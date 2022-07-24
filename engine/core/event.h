#pragma once

#include "core/math.h"
#include "event_link.h"

template <class EventData>
class BaseEvent
{
public:
	static inline Vector<BaseEventLink<EventData>*> queue;
	static void fire(const EventData& e)
	{
		for (BaseEventLink<EventData>* event_link : queue)
		{
			event_link->fire(e);
		}
	}
};

struct EventWindowResizing : public BaseEvent<EventWindowResizing>
{
	uint32_t width;
	uint32_t height;
};

struct EventWindowClosing : public BaseEvent<EventWindowClosing>
{
};
