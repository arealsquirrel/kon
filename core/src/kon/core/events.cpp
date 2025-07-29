
#include "events.hpp"
#include "kon/debug/log.hpp"

namespace kon {

EventBus::EventBus(Allocator *allocator) 
	: m_listeners(allocator), m_eventClassIDToIndex(allocator), m_allocator(allocator) {}

EventBus::~EventBus() = default;

void EventBus::register_event(UUID eventID) {
	u32 i = m_listeners.get_count();
	m_listeners.add(ArrayList<EventListener*>(m_allocator));
	m_eventClassIDToIndex.add({eventID, i});
}

void EventBus::add_listener(EventListener *listener, UUID eventID) {
	if(m_eventClassIDToIndex.has_key(eventID) == false) {
		register_event(eventID);
	}

	ArrayList<EventListener*> &arr = m_listeners[m_eventClassIDToIndex.find(eventID).second];
	arr.add(listener);
}


void EventBus::remove_listener(EventListener *listener, UUID eventID) {
	ArrayList<EventListener*> arr = m_listeners[m_eventClassIDToIndex.find(eventID).second];
	for(u32 i = 0; i < arr.get_count(); i++) {
		if(arr[i] == listener) {
			arr.erase(i);
			return;
		}
	}

	return;
}

EventListener::EventListener(EventBus &bus)
	: m_bus(bus) {}

EventListener::~EventListener() {
	for(u32 i = 0; i < m_subscribedEvents; i++) {
		m_bus.remove_listener(this, m_events[i]);
	}
}

void EventListener::subscribe_event(UUID eventID) {
	m_events[m_subscribedEvents++].uuid = eventID.uuid;
	m_bus.add_listener(this, eventID);
}

}

