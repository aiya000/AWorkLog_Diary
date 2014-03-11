#include "WorkLogData.h"

WorkLogData::WorkLogData(
		int id, time_t time,
		std::string function, std::string target,
		std::string comment)
{
	this->setId(id);
	this->setTime(time);
	this->setFunction(function);
	this->setTarget(target);
	this->setComment(comment);
}
WorkLogData::WorkLogData(
		time_t time, std::string function,
		std::string target, std::string comment)
{
	this->setId(-1);
	this->setTime(time);
	this->setFunction(function);
	this->setTarget(target);
	this->setComment(comment);
}

void WorkLogData::setId(int id)                     { m_id = id;             }
void WorkLogData::setTime(time_t time)              { m_time = time;         }
void WorkLogData::setFunction(std::string function) { m_function = function; }
void WorkLogData::setTarget(std::string target)     { m_target = target;     }
void WorkLogData::setComment(std::string comment)   { m_comment = comment;   }
int WorkLogData::getId()               { return m_id;       }
time_t WorkLogData::getTime()          { return m_time;     }
std::string WorkLogData::getFunction() { return m_function; }
std::string WorkLogData::getTarget()   { return m_target;   }
std::string WorkLogData::getComment()  { return m_comment;  }

#include <iostream>
void WorkLogData::showData(){
	// TODO: リリース時にこの関数消す。
	std::cout << ">> DB Data Showing"  << std::endl
	          << "time: "              << this->getTime()     << std::endl
	          << "func: "              << this->getFunction() << std::endl
	          << "target: "            << this->getTarget()   << std::endl
	          << "comment: "           << this->getComment()  << std::endl;
}
