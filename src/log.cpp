#include "log.h"

#include <iostream>
#include <string>
#include <iomanip>

// TODO change to preprocessor
static std::string LOG_TYPE_NAME[] =
{
	"DEFAULT",
	"DEBUG",
	"INIT",
	"WARNING",
	"ERROR",
};

int nrLogTypes = LOG_ERROR - LOG_DEFAULT + 1;
std::vector<std::vector<std::string>> Log::m_LogEntries = std::vector<std::vector<std::string>>(nrLogTypes);

void Log::Message(const std::string& message, const LOG_TYPE type)
{
	m_LogEntries[type].push_back(message);
}

void Log::Clear()
{
	for (auto& entry : m_LogEntries) entry.clear();
}

void Log::Display(const LOG_TYPE type)
{
	const std::string divider = "=========================================================";
	const bool filter = type != LOG_DEFAULT;

	for (std::size_t i = 0; i < m_LogEntries.size(); ++i)
	{
		if (m_LogEntries.at(i).size() > 0)
		{
			if (!filter || (filter && i == type))
			{
				std::cout << divider << std::endl;
				std::cout << LOG_TYPE_NAME[i] << std::endl;
				std::cout << divider << std::endl;
				for (std::size_t j = 0; j < m_LogEntries.at(i).size(); ++j)
				{
					std::cout << std::setw(7) << LOG_TYPE_NAME[i] << ": " << m_LogEntries.at(i).at(j) << std::endl;
					std::cout << " - "  << m_LogEntries[i][j] << std::endl;
				}
				std::cout << std::endl;
				std::cout << divider << std::endl << std::endl;
			}
		}
	}
}