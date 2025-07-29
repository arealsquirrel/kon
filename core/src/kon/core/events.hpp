#ifndef KN_EVENTS_HPP
#define KN_EVENTS_HPP

#include "kon/container/array.hpp"
#include "kon/container/arraylist.hpp"
#include "kon/container/set.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/uuid.hpp"
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

class EventListener {
public:
	EventListener(EventBus &bus);
	virtual ~EventListener();

public:
	template<class E>
	void subscribe_event() {
		subscribe_event(E::get_static_uuid());
	}

	void subscribe_event(UUID eventID);

	virtual void on_event(Event &event) = 0;

private:
	EventBus &m_bus;
	u32 m_subscribedEvents {0};
	UUID m_events[32];
};


class EventBus {
public:
	EventBus(Allocator *allocator);
	~EventBus();

public:
	void register_event(UUID eventID);
	void add_listener(EventListener *listener, UUID eventID);
	void remove_listener(EventListener *listeners, UUID eventID);

	template<class E>
	void add_listener(EventListener *listener) {
		add_listener(listener, E::get_static_uuid());
	}

	template<class E>
	void remove_listener(EventListener *listener) {
		remove_listener(listener, E::get_static_uuid());
	}

	template<class E, typename ...Args>
	void emit_event(Args &&...args) {
		E event(std::forward<Args>(args)...);

		ArrayList<EventListener*> arr = m_listeners[m_eventClassIDToIndex.find(
				E::get_static_uuid()).second];
		arr.for_each([&](EventListener *l, u32){
			l->on_event(event);
		});
	}

private:
	ArrayList<ArrayList<EventListener*>> m_listeners;
	Set<UUID, u32> m_eventClassIDToIndex;

	Allocator *m_allocator;
};

}

#endif
