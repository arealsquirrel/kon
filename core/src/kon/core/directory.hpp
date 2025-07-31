#ifndef KN_DIRECTORY_HPP
#define KN_DIRECTORY_HPP

#include "kon/container/arraylist.hpp"
#include "kon/container/string.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/core.hpp"

namespace kon {


enum FilePermissions : u8 {
	FilePermissions_None = KN_BIT(0),
	FilePermissions_Read = KN_BIT(1),
	FilePermissions_Write = KN_BIT(2),
	FilePermissions_Execute = KN_BIT(3)
};

struct PathStat {
	bool valid;
	bool directory;
	u32 size;
	u8 permissions;
};

class Directory;

namespace platform {

PathStat get_path_stat(const char *path);
void iterate_directory(Allocator *allocator, const char *path, ArrayList<Directory> &arr);

}


/*
 * defines a platform dependent directory class
 */
class Directory {
public:
	Directory(String path);
	Directory(const char *str, Allocator *allocator);
	Directory(const Directory &directory);
	~Directory();

public:
	PathStat get_stat() const { return m_stat; }
	bool get_valid() const { return m_stat.valid; }

	bool operator ==(bool b) { return m_stat.valid = b; }

	const String &get_string() const { return m_path; }

	ShortString get_file_name() const;
	ShortString get_file_extension() const;

	Directory operator +(const char *str) {
		String cpy(m_path.c_str(), m_path.get_allocator());
		return Directory(cpy.append(str));
	}

	Directory operator +(String str) {
		return *this + str.c_str();
	}

	Directory operator +(ShortString str) {
		return *this + str.c_str();
	}

	Directory operator +(Directory dir) {
		return *this + dir.m_path.c_str();
	}

	void operator +=(const char *str) {
		m_path.append(str);
		m_stat = platform::get_path_stat(m_path.c_str());
	}

	const char *c_str() const { return m_path.c_str(); }

private:
	PathStat m_stat;
	String m_path;
};

}

#endif
