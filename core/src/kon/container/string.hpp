#ifndef KN_STRING_HPP
#define KN_STRING_HPP

#include "kon/core/allocator.hpp"
#include "kon/core/assert.hpp"
#include "kon/core/util.hpp"
#include <cstring>
#include <kon/core/core.hpp>
#include <utility>

namespace kon {

/*
 * abstract class that has all the string stuff,
 * but we have subclasses that handles the stack+heap strings
 */

/*
class ImplString {
public:
	ImplString() = default;
	~ImplString() = default;
	ImplString(const ImplString &imp) = default;

public:
	inline u32 compare(const char *str) const { return strcmp(get_buffer(), str); }
	inline bool equals(const char *str) const { return (compare(str) == 0); }

	inline char char_at(u32 index) const { return get_buffer()[index]; }

	inline int index_of_char(char c) const {
		char *f = strchr(get_buffer(), c);
		if(f == nullptr) return -1;
		return (int)(f - get_buffer());
	}

	inline int index_of_string(const char *c) { 
		char *f = strstr(get_buffer(), c);
		if(f == nullptr) return -1;
		return (int)(f - get_buffer());
	}

public:
	const char *c_str() const { return get_buffer(); }
	virtual u32 get_size() const = 0;

private:
	virtual char *get_buffer() const = 0;
	virtual void resize(u32 size) = 0;
};
*/

class ShortString;

class String { // : public ImplString {
public:
	String(Allocator *allocator);
	String(const char *string, Allocator *allocator);
	String(const String &string);
	~String();

	void operator =(const char *str) {
		u32 s = strlen(str);
		resize(s+1);
		strcpy(m_buffer, str);
		m_buffer[s] = '\0';
	}

	void resize(u32 size);

	u32 get_size() const { return m_size; }
	const char *c_str() const { return m_buffer; }

	Allocator *get_allocator() const { return m_allocator; }

	String substring(int a, int b) const;
	ShortString short_substring(int a, int b) const;

	inline u32 compare(const char *str) const { return strcmp(m_buffer, str); }
	inline bool equals(const char *str) const { return (compare(str) == 0); }

	inline char char_at(u32 index) const { return m_buffer[index]; }

	inline int index_of_char(char c) const {
		char *f = strchr(m_buffer, c);
		if(f == nullptr) return -1;
		return (int)(f - m_buffer);
	}

	inline int index_of_string(const char *c) { 
		char *f = strstr(m_buffer, c);
		if(f == nullptr) return -1;
		return (int)(f - m_buffer);
	}

public:
	String &append(const char *str);

private:
	u32 m_size {0};
	char *m_buffer {nullptr};
	Allocator *m_allocator;
};

/*
 * now it is constexpr for the most part
 */
class ShortString {
public:
	ShortString() = default;
	constexpr ShortString(const char *string) { *this = string; }
	ShortString(const ShortString &string) = default;
	~ShortString() = default;

	constexpr void operator =(const char *str) {
		strcpy(m_buffer, str);
	}

	ShortString substring(int a, int b) const;

	u32 get_size() const { return strlen(m_buffer); }
	const char *c_str() const { return m_buffer; }

	ShortString short_substring(int a, int b) const;

	inline u32 compare(const char *str) const { return strcmp(m_buffer, str); }
	inline bool equals(const char *str) const { return (compare(str) == 0); }

	inline char char_at(u32 index) const { return m_buffer[index]; }

	inline int index_of_char(char c) const {
		const char *f = strchr(&m_buffer[0], c);
		if(f == nullptr) return -1;
		return (int)(f - m_buffer);
	}

	inline int index_of_string(const char *c) { 
		char *f = strstr(m_buffer, c);
		if(f == nullptr) return -1;
		return (int)(f - m_buffer);
	}

	 u64 hash() const;

public:
	ShortString &append(const char *str);

private:
	char m_buffer[256] {};
};

}

namespace std {
	template<>
	struct hash<kon::ShortString> {
		std::size_t operator()(const kon::ShortString &string) const {
			return string.hash();
		}
	};
}

#endif
