
#include "kon/container/string.hpp"
#include "kon/core/assert.hpp"
#include "kon/debug/log.hpp"
#include <kon/core/directory.hpp>
#include <sys/stat.h>
#include <dirent.h>

namespace kon {
namespace platform {

PathStat get_path_stat(const char *path) {
	PathStat s;
	struct stat statbuf;
	if(stat(path, &statbuf) == -1) {
		s.valid = false;
		return s;
	}
	
	s.valid = true;
	s.size = statbuf.st_size;
	s.directory = S_ISDIR(statbuf.st_mode);
	s.permissions |= (statbuf.st_mode & S_IRUSR) ? FilePermissions_Read : FilePermissions_None;
	s.permissions |= (statbuf.st_mode & S_IWUSR) ? FilePermissions_Write : FilePermissions_None;
	s.permissions |= (statbuf.st_mode & S_IEXEC) ? FilePermissions_Execute : FilePermissions_None;
	
	return s;
}

void iterate_directory(
		Allocator *allocator, const char *path, ArrayList<Directory> &arr) {

	KN_ASSERT(get_path_stat(path).valid, "path is not valid");
	KN_ASSERT(get_path_stat(path).directory, "path is not a directory");

	struct dirent *dp;
	DIR *dir = opendir(path);
	while((dp = readdir(dir))) {
		kon::String p(path, allocator);
		p.append(dp->d_name);
		arr.add(Directory(p));
	}
	closedir(dir);
}

}
}

