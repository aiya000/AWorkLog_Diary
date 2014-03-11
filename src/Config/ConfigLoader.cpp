#include "ConfigLoader.h"
#include "../Lib/StringUtils.hpp"
#include <cstdlib>

ConfigLoader::ConfigLoader() :
	APP_DIR_NAME(".aworklog_diary"),
	m_workDirPath(  alib::format("%s/%s",    std::getenv("HOME"), APP_DIR_NAME.c_str()) ),
	m_dbPath(       alib::format("%s/%s/%s", std::getenv("HOME"), APP_DIR_NAME.c_str(), "aworklog_diary.db") ){}

std::string ConfigLoader::getWorkDirPath(){
	return m_workDirPath;
}
std::string ConfigLoader::getDbPath(){
	return m_dbPath;
}
