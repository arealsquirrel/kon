
#include <kon/core/directory.hpp>
#include <sys/stat.h>
#include <dirent.h>

namespace kon {

Directory::Directory(String path)
	: m_path(path) {
	
	struct stat statbuf;
	if(stat(path.c_str(), &statbuf) == -1) {
		m_valid = false;
		return;
	}
	
	m_valid = true;
	m_stat.size = statbuf.st_size;
	m_stat.directory = S_ISDIR(statbuf.st_mode);
	m_stat.permissions |= (statbuf.st_mode & S_IRUSR) ? FilePermissions_Read : FilePermissions_None;
	m_stat.permissions |= (statbuf.st_mode & S_IWUSR) ? FilePermissions_Write : FilePermissions_None;
	m_stat.permissions |= (statbuf.st_mode & S_IEXEC) ? FilePermissions_Execute : FilePermissions_None;
}

Directory::Directory(const Directory &directory)
	: m_stat(directory.m_stat), m_path(directory.m_path) {}

Directory::~Directory() = default;

}

