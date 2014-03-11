#pragma once
#include <string>

class ConfigLoader {
private:
	const std::string APP_DIR_NAME;
	std::string m_workDirPath;
	std::string m_dbPath;

public:
	ConfigLoader();
	std::string getWorkDirPath();
	std::string getDbPath();
};
