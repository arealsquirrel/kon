#ifndef KN_HASHMAP_HPP
#define KN_HASHMAP_HPP

#include "kon/core/allocator.hpp"
#include "kon/core/util.hpp"
#include "kon/debug/log.hpp"
#include <functional>
#include <kon/core/core.hpp>
#include <limits>
#include <utility>

namespace kon {


/*
 * the Key type needs to support std::hash
 * you can do the [const Key] to get values
 *
 * this is implemented as a robinmap btw
 */
template<class Key, class Value>
class HashMap {
public:

	/* how many elements are stored in each bucket */
	static constexpr u32 c_skipIndex = 5;
	static constexpr u32 c_max_psl = std::numeric_limits<u32>::max();

public:
	struct Node {
		Pair<const Key, Value> pair;
		u32 psl;
	};

public:
	/*
	 * the elements field is how many elements
	 * you estimate will be in here. Its gonna
	 * use the load index to get the actual size
	 */
	HashMap(Allocator *allocator, u32 elements)
		: m_allocator(allocator) {
	
		m_buckets = elements / 0.75f;
		m_nodes = reinterpret_cast<Node*>(
				m_allocator->allocate_mem(m_buckets * c_skipIndex * sizeof(Node)));

		for (u32 i = 0; i < m_buckets*c_skipIndex; i++) {
			m_nodes[i].psl = c_max_psl;
		}
	}

	~HashMap() {
		for_each([](auto &pair) {
			pair.first.~Key();
			pair.second.~Value();
		});

		m_allocator->free_mem(KN_MEM_POINTER(m_nodes), get_size() * sizeof(Node));
	}

public:
	Pair<const Key, Value> &add(const Pair<const Key, Value> &entry) {
		auto &tempEntry = entry;
		u32 hash = m_hash(entry.first);
		u32 entryIndex = (hash % m_buckets) * c_skipIndex;
		u32 vpsl = 0;

		while(m_nodes[entryIndex].psl != c_max_psl) {
			if(vpsl > m_nodes[entryIndex].psl) {


				/*
				 * ERRRMM tHis CoDe iSnT SafE
				 *
				 * shut the fuck up.
				 * its wonderfull.
				 * I am free and I choose to not do a damn thing about this code
				 */
				std::swap(const_cast<Value&>(tempEntry.second),
						  const_cast<Value&>(m_nodes[entryIndex].pair.second));

				std::swap(const_cast<Key&>(tempEntry.first),
						  const_cast<Key&>(m_nodes[entryIndex].pair.first));

				std::swap(vpsl, m_nodes[entryIndex].psl);
			}

			entryIndex = (entryIndex+1) % get_size();
			vpsl++;
		}

		Node *n = new (m_nodes+entryIndex) Node{entry, vpsl};
		return n->pair;
	}

	void remove(const Key &key) {
		u32 entryIndex = find_entry_index(key);
		
		m_nodes[entryIndex].pair.first.~Key();
		m_nodes[entryIndex].pair.second.~Value();
		m_nodes[entryIndex].psl = c_max_psl;

		entryIndex = (entryIndex+1) % get_size();
		while(m_nodes[entryIndex].psl != c_max_psl) {
			if(m_nodes[entryIndex].psl > 0) {
				m_nodes[entryIndex].psl -= 1;

				std::swap(const_cast<Value&>(m_nodes[entryIndex-1].pair.second),
					const_cast<Value&>(m_nodes[entryIndex].pair.second));

				std::swap(const_cast<Key&>(m_nodes[entryIndex-1].pair.first),
						const_cast<Key&>(m_nodes[entryIndex].pair.first));


				std::swap(m_nodes[entryIndex].psl, m_nodes[entryIndex-1].psl);
			} else {
				return;
			}

			entryIndex = (entryIndex+1) % get_size();
		}
	}

	inline Pair<const Key, Value> &find_entry(const Key &key) {
		return m_nodes[find_entry_index(key)].pair;
	}
	
	/*
	 * this thing is special because its foreach
	 * does not fuck with indexes
	 */
	void for_each(std::function<void( Pair<const Key, Value>& )> func) {
		for(u32 i = 0; i < get_size(); i++) {
			if(m_nodes[i].psl != c_max_psl) {
				func(m_nodes[i].pair);
			}
		}
	}

public:
	inline u32 get_count() const { return m_count; }
	inline u32 get_size() const { return m_buckets * c_skipIndex; }
	inline float load_factor() { return ((float)m_count / (m_buckets * c_skipIndex)); }

	bool has_key(const Key &key) const { return (find_entry_index(key) != c_max_psl); }

	Node *get_array() const { return m_nodes; }

	Pair<const Key, Value> &operator[](const Key &key) { return find_entry(key); }

private:
	inline u32 find_entry_index(const Key &key) const {
		u32 hash = m_hash(key);
		u32 entryIndex = (hash % m_buckets) * c_skipIndex;
		u32 vpsl = 0;

		while(m_nodes[entryIndex].psl != c_max_psl) {
			if(vpsl > m_nodes[entryIndex].psl) {
				 return c_max_psl;
			}

			if(hash == m_hash(m_nodes[entryIndex].pair.first)) {
				return entryIndex;
			}

			entryIndex = (entryIndex+1) % get_size();
			vpsl++;
		}

		return c_max_psl;
	}

private:
	Allocator *m_allocator;

	u32 m_buckets;
	u32 m_count {0};

	std::hash<Key> m_hash;

	Node *m_nodes;
};

}

#endif
