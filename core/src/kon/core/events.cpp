
#include "events.hpp"
#include "kon/core/allocator.hpp"
#include "kon/debug/log.hpp"
#include <kon/engine/engine.hpp>

namespace kon {

EventListener::EventListener(Object *obj) 
	: m_attached(obj),
	m_attachedEvents(obj->get_engine()->get_allocator_persistent(), 2),
	m_bus(obj->get_engine()->get_event_bus()) {}

EventListener::~EventListener() {
	m_attachedEvents.for_each([&](UUID id, u32){
		m_bus.destroy_callback(id, m_attached->get_instance_id());
	});
}

EventBus::EventBus(Allocator *allocator) 
	: m_listeners(allocator), m_eventClassIDToIndex(allocator), m_allocator(allocator) {}

EventBus::~EventBus() = default;

void EventBus::register_event(UUID eventID) {
	u32 i = m_listeners.get_count();
	m_listeners.add(ArrayList<EventCallback>(m_allocator));
	m_eventClassIDToIndex.add({eventID, i});
}

void EventBus::add_listener(EventCallback function, UUID eventID) {
	if(m_eventClassIDToIndex.has_key(eventID) == false) {
		register_event(eventID);
	}

	ArrayList<EventCallback> &arr = m_listeners[m_eventClassIDToIndex.find(eventID).second];
	arr.add(function);
}

void EventBus::destroy_callback(UUID eventID, UUID instanceID) {
	if(m_eventClassIDToIndex.has_key(eventID) == false) return;
	ArrayList<EventCallback> &arr = m_listeners[m_eventClassIDToIndex.find(eventID).second];

	for(u32 i = 0; i < arr.get_count(); i++) {
		if(arr[i].instanceID == instanceID) {
			arr.erase(i);
			i--;
		}
	}
}

}

