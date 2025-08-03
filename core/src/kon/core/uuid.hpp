#ifndef KN_UUID_HPP
#define KN_UUID_HPP

#include "kon/core/core.hpp"
#include <cstddef>
#include <functional>

namespace kon {

struct UUID {
public:
	UUID();
	UUID(const UUID &id);
	UUID(u64 id);
	~UUID();

	void operator =(const UUID &id) {
		uuid = id.uuid;
	}

	operator u64() const { return uuid; }
	
	bool operator ==(const UUID &_uuid) { return (_uuid.uuid == uuid); }

public:
	u64 uuid;
};

}

namespace std {
	template<>
	struct hash<kon::UUID> {
		std::size_t operator()(const kon::UUID &uuid) const {
			return uuid.uuid;
		}
	};
}

#endif
