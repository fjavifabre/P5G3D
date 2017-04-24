#pragma once

#include <vector>

enum LOG_TYPE
{
	LOG_DEFAULT = 0,
	LOG_DEBUG	= 1,
	LOG_INIT	= 2,
	LOG_WARNING = 3,
	LOG_ERROR	= 4,
};


class Log
{
private:

	// Continuos vector of vector of logs VS maps of logs
	static std::vector<std::vector<std::string>> m_LogEntries;

public:

	//logs a user / engine specified message with optional
	// type of log message. Note that log messages are queued and 
	// flushed/displyayed once by calling Flush.
	static void Message(const std::string& message, const LOG_TYPE type = LOG_DEFAULT);

	//Clear all log entries
	static void Clear();

	static void Display(const LOG_TYPE type = LOG_DEFAULT);

};