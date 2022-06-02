#pragma once

#include "common.h"

template <class EventData>
class BaseEventLink
{
public:
	virtual void fire(const EventData& e) = 0;
};

template <class EventReceiverClass, class EventData>
class EventLink : public BaseEventLink<EventData>
{
	using EventHandler = void (EventReceiverClass::*)(const EventData&);

public:
	EventLink(EventReceiverClass* handling_object, EventHandler event_handler)
	    : handler(event_handler)
	    , object(handling_object)
	{
		// Establish link
		EventData::queue.push_back(this);
	}

	~EventLink()
	{
		// Break link since we can't receive events now
		for (int i = 0; i < EventData::queue.size(); i++)
		{
			if (EventData::queue.at(i) == this)
			{
				EventData::queue.erase(EventData::queue.begin() + i);
			}
		}
	}

	EventLink(const EventLink&) = delete;
	EventLink(EventLink&&) = delete;
	EventLink& operator=(const EventLink&) = delete;
	EventLink& operator=(EventLink&&) = default;

	void fire(const EventData& e) override { (object->*handler)(e); }

private:
	EventReceiverClass* object = nullptr;
	EventHandler handler;
};
