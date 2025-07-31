
#include "directory.hpp"

namespace kon {


Directory::Directory(String path)
	: m_stat(platform::get_path_stat(path.c_str())),
	  m_path(path.c_str(), path.get_allocator()) {}

Directory::Directory(const Directory &directory)
	: m_stat(directory.m_stat), m_path(directory.m_path) {}

Directory::~Directory() = default;

}

