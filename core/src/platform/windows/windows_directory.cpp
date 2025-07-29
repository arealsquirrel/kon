
#include <kon/core/directory.hpp>

#error whyy

namespace kon {

Directory::Directory(String path)
	: m_path(path) {}

Directory::Directory(const Directory &directory)
	: m_path(directory.m_path) {}

Directory::~Directory() {

}

}
