#ifndef KN_EVENTS_HPP
#define KN_EVENTS_HPP

#include "kon/container/array.hpp"
#include "kon/container/arraylist.hpp"
#include "kon/container/set.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/object.hpp"
#include "kon/core/uuid.hpp"
#include <functional>
#include <kon/core/core.hpp>

namespace kon {

#define KN_EVENT public: \
	static UUID get_static_uuid() { static ::kon::UUID id; return id; } \
	virtual UUID get_uuid() const override { return get_static_uuid(); }

class Event {
public:
	Event() = default;
	virtual ~Event() = default;

public:
	virtual UUID get_uuid() const = 0;
};

class EventBus;

using EventFunction = std::function<void(Event &event)>;

struct EventCallback {
	EventFunction m_callback;
	UUID instanceID;
};

class EventBus {
public:
	EventBus(Allocator *allocator);
	~EventBus();

public:
	void add_listener(EventCallback function, UUID eventID);

	void register_event(UUID eventID);

	template<class E, typename ...Args>
	void emit_event(Args &&...args) {
		E event(std::forward<Args>(args)...);

		ArrayList<EventCallback> arr = m_listeners[m_eventClassIDToIndex.find(
				E::get_static_uuid()).second];

		arr.for_each([&](EventCallback &l, u32){
			l.m_callback(event);
		});
	}

	void destroy_callback(UUID eventID, UUID instanceID);

private:
	ArrayList<ArrayList<EventCallback>> m_listeners;
	Set<UUID, u32> m_eventClassIDToIndex;

	Allocator *m_allocator;
};

class EventListener {
public:
	EventListener(Object *obj);
	virtual ~EventListener();

public:
	template<class E>
	void subscribe_event(std::function<void(E &event)> eventFunction) {
		m_bus.add_listener(
				EventCallback { 
				// Event callback function
				[eventFunction](Event &event){ 
					eventFunction(dynamic_cast<E&>(event)); 
				}, 

				// instance ID
				m_attached->get_instance_id()
			}, E::get_static_uuid());

		m_attachedEvents.add(E::get_static_uuid());
	}

private:
	Object *m_attached;
	ArrayList<UUID> m_attachedEvents;
	EventBus &m_bus;
};


// ----------- ENGINE EVENTS ----------- //
struct EventEngineExit : public Event {
KN_EVENT

public:
	EventEngineExit() = default;
	~EventEngineExit() = default;
};

}

#endif
