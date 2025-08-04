
#include "directory.hpp"

namespace kon {


Directory::Directory(const String &path)
	: m_stat(platform::get_path_stat(path.c_str())),
	  m_path(path) {}

Directory::Directory(const char *str, Allocator *allocator)
	: m_stat(platform::get_path_stat(str)), m_path(str, allocator) {}

Directory::Directory(const Directory &directory)
	: m_stat(directory.m_stat), m_path(directory.m_path) {}

Directory::~Directory() = default;

ShortString Directory::get_file_name() const {
	int i = m_path.get_size();
	for (; i >= 0; i--) {
		if(m_path.char_at(i) == '/') break;
	}

	return m_path.short_substring(i+1, m_path.get_size());
}

ShortString Directory::get_file_extension() const {
	int i = m_path.get_size();
	for (; i >= 0; i--) {
		if(m_path.char_at(i) == '.') break;
	}

	return m_path.short_substring(i+1, m_path.get_size());
}

}

