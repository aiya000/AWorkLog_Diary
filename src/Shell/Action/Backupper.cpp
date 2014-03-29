#include "Backupper.h"
#include <fstream>
#include "../../Lib/Convert.hpp"

Backupper::Backupper(){
	this->reset();
}
void Backupper::reset(){
	m_backupFiles.clear();

	alib::CDirectory dir(m_config.getWorkDirPath());
	std::vector<std::string> paths = dir.getSubFiles();

	for(int i=0; i<paths.size(); i++)
		m_backupFiles.insert( FileList::value_type(i+1, paths[i]) );
}

void Backupper::backup() throw(alib::FileIOException){
	std::ifstream fin(m_config.getDbPath(), std::ios::in|std::ios::binary);
	std::string backupName = alib::timeToString(time(nullptr), "%Y-%m-%d");
	std::ofstream fout(m_config.getDbPath()+"."+backupName, std::ios::out|std::ios::binary);
	if( fin.fail() || fout.fail() )
		throw alib::FileIOException();
	fout << fin.rdbuf();
}

void Backupper::restore(int id) throw(alib::FileIOException){
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

void Backupper::list(){
	FileList::iterator files = m_backupFiles.begin();
	while(files != m_backupFiles.end()){
		std::cout << files->first  << "|\t"
			      << files->second << std::endl;
		files++;
	}
}
