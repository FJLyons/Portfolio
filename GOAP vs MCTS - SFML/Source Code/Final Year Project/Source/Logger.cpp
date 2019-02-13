#include "Logger.h"

Logger* Logger::loggerInstance = nullptr;

Logger::Logger()
{
	headers = "Game Number,Winner,No. of Wins,Game Time,GOAP Highest Value,GOAP Highest Attack Value,GOAP Highest Defence Value,GOAP Highest Value Time Stamp,GOAP Total Builders,GOAP Total Soldiers,GOAP Total Archers,GOAP Total Walls,MCTS Highest Value,MCTS Highest Attack Value,MCTS Highest Defence Value,MCTS Highest Value Time Stamp,MCTS Total Builders,MCTS Total Soldiers,MCTS Total Archers,MCTS Total Walls,Game Over Time Stamp";

	results.push_back(headers);
}

Logger::~Logger()
{
}

Logger * Logger::GetInstance()
{
	if (loggerInstance == nullptr)
	{
		loggerInstance = new Logger();
	}
	return loggerInstance;
}

std::string Logger::GetTimeStamp(const bool save)
{
	time_t t = time(0);
	struct tm now;

	localtime_s(&now, &t);

	std::string timeStamp;

	if (save == true)
	{
		timeStamp = ""
			+ std::to_string(now.tm_mday) + "-"
			+ std::to_string(now.tm_mon + 1) + "-"
			+ std::to_string(now.tm_year + 1900) + "_"
			+ std::to_string(now.tm_hour) + "-"
			+ std::to_string(now.tm_min) + "-"
			+ std::to_string(now.tm_sec);
	}
	else
	{
		timeStamp = ""
			+ std::to_string(now.tm_mday) + "/"
			+ std::to_string(now.tm_mon + 1) + "/"
			+ std::to_string(now.tm_year + 1900) + " - "
			+ std::to_string(now.tm_hour) + ":"
			+ std::to_string(now.tm_min) + ":"
			+ std::to_string(now.tm_sec);
	}

	return timeStamp;
}

void Logger::LogGame(std::string game)
{
	results.push_back(game);
}

void Logger::SaveFile()
{
	logFileName = "Game Results = "
		+ GetTimeStamp(true)
		+ ".csv";

	std::string log;

	for (int i = 0; i < results.size(); i++)
	{
		log += results[i] + "\n";
	}

	std::ofstream output_file(logFileName);
	output_file << log;
	results.clear();
	log = "";
	results.push_back(headers);
}
