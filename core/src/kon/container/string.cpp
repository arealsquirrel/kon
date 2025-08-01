
#include "string.hpp"
#include "kon/core/allocator.hpp"
#include <algorithm>
#include <cstring>

namespace kon {

String &String::append(const char *str) {
	size_t len = strlen(str);
	resize(len + get_size());
	strcat(get_buffer(), str);
	return *this;
}

String::String(Allocator *allocator)
	: m_allocator(allocator) {}

String::String(const String &string)
	: m_allocator(string.m_allocator) {

	*this = string.c_str();
}

String::String(const char *string, Allocator *allocator)
	: m_allocator(allocator) {
	*this = string;
}

String::~String() {
	m_allocator->free_mem(m_buffer, m_size);
}

void String::resize(u32 size) {
	if(m_buffer == nullptr) {
		m_buffer = m_allocator->allocate_mem(size+1);
		m_size = size;
		return;
	}

	char *temp = m_allocator->allocate_mem(size+1);
	memcpy(temp, m_buffer, std::min(size, m_size));

	m_allocator->free_mem(m_buffer, m_size+1);

	m_buffer = temp;
	m_size = size;
}

String String::substring(int a, int b) const {
	String str(m_allocator);
	str.resize(b-a+1);
	memcpy(str.m_buffer, m_buffer+a, b-a);
	str.m_buffer[b-a] = '\0';
	return str;
}

ShortString String::short_substring(int a, int b) const {
	ShortString s;
	memcpy(s.get_buffer(), m_buffer+a, b-a);
	return s;
}

ShortString::ShortString(const char *string) {
	*this = string;
}

ShortString &ShortString::append(const char *str) {
	strcat(get_buffer(), str);
	return *this;
}


ShortString ShortString::substring(int a, int b) const {
	ShortString s;
	memcpy(s.m_buffer, m_buffer+a, b-a+1);
	s.m_buffer[b-a] = '\0';
	return s;
}

u64 ShortString::hash() const {
	const char *s = &m_buffer[0];
	unsigned h = 37;
   	while (*s) {
   		h = (h * 54059) ^ (s[0] * 76963);
		s++;
   	}
   	return h; // or return h % C;
}

}

