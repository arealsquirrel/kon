
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

ShortString::ShortString(const char *string) {
	*this = string;
}

ShortString &ShortString::append(const char *str) {
	strcat(get_buffer(), str);
	return *this;
}

}

