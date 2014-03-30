#include "BackupHelper.h"
#include <algorithm>
#include <fstream>
#include "../../Lib/Convert.hpp"
#include <cstdio>

BackupHelper::BackupHelper(){
	this->reset();
}
void BackupHelper::reset(){
	m_backupFiles.clear();

	alib::CDirectory dir(m_config.getWorkDirPath());
	std::vector<std::string> paths = dir.getSubFiles();
	std::sort(paths.begin(), paths.end());

	for(int i=0; i<paths.size(); i++)
		if(i != 0)  // Evacuate Currency
			m_backupFiles.insert( FileList::value_type(i, paths[i]) );
}

void BackupHelper::backup() throw(alib::FileIOException){
	std::ifstream fin(m_config.getDbPath(), std::ios::in|std::ios::binary);
	std::string backupName = alib::timeToString(time(nullptr), "%Y-%m-%d");
	std::ofstream fout(m_config.getDbPath()+"."+backupName, std::ios::out|std::ios::binary);
	if( fin.fail() || fout.fail() )
		throw alib::FileIOException();
	fout << fin.rdbuf();
}

void BackupHelper::restore(int id) throw(alib::FileIOException){
	std::string restoreName = "not found";
	FileList::iterator files = m_backupFiles.begin();
	while(files != m_backupFiles.end()){
		if(files->first == id){
			restoreName = files->second;
			break;
		}
		files++;
	}
	if(restoreName == "not found")
		throw alib::FileIOException();

	restoreName = m_config.getWorkDirPath() + "/" + restoreName;
	std::ifstream fin(restoreName, std::ios::in|std::ios::binary);
	std::ofstream fout(m_config.getDbPath(), std::ios::out|std::ios::binary);
	if( fin.fail() || fout.fail() )
		throw alib::FileIOException();
	fout << fin.rdbuf();
}

void BackupHelper::remove(int id) throw(alib::FileIOException){
	if(m_backupFiles.count(id) != 1)
		throw alib::FileIOException(">> No such Backup FileID.");

	int succeed = std::remove(m_backupFiles[id].c_str());
	if(succeed != 0)
		throw alib::FileIOException(">> File Remove Failed.");
	this->reset();
}

void BackupHelper::list(){
	FileList::iterator files = m_backupFiles.begin();
	while(files != m_backupFiles.end()){
		std::cout << files->first  << "|\t"
		          << files->second << std::endl;
		files++;
	}
}
