#pragma once
#include <ctime>
#include <string>

class WorkLogData {
private:
	int m_id;
	time_t m_time;
	std::string m_function;
	std::string m_target;
	std::string m_comment;
public:
	WorkLogData(int id, time_t time,
			std::string function, std::string target,
			std::string comment);
	WorkLogData(time_t time, std::string function,
			std::string target, std::string comment);
	void setId(int id);
	void setTime(time_t time);
	void setFunction(std::string function);
	void setTarget(std::string target);
	void setComment(std::string comment);
	int getId();
	time_t getTime();
	std::string getFunction();
	std::string getTarget();
	std::string getComment();

	void showData();
};
