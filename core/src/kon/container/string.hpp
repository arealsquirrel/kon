#ifndef KN_STRING_HPP
#define KN_STRING_HPP

#include "kon/core/allocator.hpp"
#include "kon/core/assert.hpp"
#include <cstring>
#include <kon/core/core.hpp>
#include <utility>

namespace kon {

/*
 * abstract class that has all the string stuff,
 * but we have subclasses that handles the stack+heap strings
 */
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

private:
	virtual char *get_buffer() const = 0;
	virtual void resize(u32 size) = 0;
	virtual u32 get_size() = 0;
};

class String : public ImplString {
public:
	String(Allocator *allocator);
	String(const char *string, Allocator *allocator);
	String(const String &string);
	~String();

	void operator =(const char *str) {
		u32 s = strlen(str);
		resize(s+1);
		strcpy(get_buffer(), str);
		m_buffer[s] = '\0';
	}

	void resize(u32 size) override;

	char *get_buffer() const override { return m_buffer; }
	u32 get_size() override { return m_size; }

public:
	String &append(const char *str);

private:
	u32 m_size {0};
	char *m_buffer {nullptr};
	Allocator *m_allocator;
};

class ShortString : public ImplString {
public:
	ShortString() = default;
	ShortString(const char *string);
	ShortString(const ShortString &string) = default;
	~ShortString() = default;

	void operator =(const char *str) {
		u32 s = strlen(str);
		KN_ASSERT(s < 256, "ShortString is larger than 256");
		strcpy(get_buffer(), str);
	}

	char *get_buffer() const override { return const_cast<char*>(&m_buffer[0]); }
	u32 get_size() override { return 256; }
	void resize(u32) override {}

public:
	ShortString &append(const char *str);

private:
	char m_buffer[256];
};

}

#endif
