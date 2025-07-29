#ifndef KN_DIRECTORY_HPP
#define KN_DIRECTORY_HPP

#include "kon/container/string.hpp"
#include "kon/core/core.hpp"

namespace kon {

enum FilePermissions : u8 {
	FilePermissions_None = KN_BIT(0),
	FilePermissions_Read = KN_BIT(1),
	FilePermissions_Write = KN_BIT(2),
	FilePermissions_Execute = KN_BIT(3)
};

struct PathStat {
	bool directory;
	u32 size;
	u8 permissions;
};

/*
 * defines a platform dependent directory class
 */
class Directory {
public:
	Directory(String path);
	Directory(const Directory &directory);
	~Directory();

public:
	PathStat get_stat() const { return m_stat; }
	bool get_valid() const { return m_valid; }

private:
	bool m_valid;
	PathStat m_stat;
	String m_path;
};

}

#endif
