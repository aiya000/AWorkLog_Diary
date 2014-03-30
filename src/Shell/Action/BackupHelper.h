#pragma once
#include "../../Config/ConfigLoader.h"
#include <map>
#include <iostream>
#include <string>
#include "../../Lib/FileSystem.hpp"

class BackupHelper {
private:
	ConfigLoader m_config;
	typedef std::map<int,std::string> FileList;
	FileList m_backupFiles;

public:
	BackupHelper();
private:
	void reset();

public:
	void backup() throw(alib::FileIOException);
	void restore(int id) throw(alib::FileIOException);
	void remove(int id)  throw(alib::FileIOException);
	void list();
};
