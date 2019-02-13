#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

class Logger
{
public:
	// Return an instance of the Logger class
	static Logger * GetInstance();
private:
	// Logger instance object - for returning instance
	static Logger * loggerInstance;

	// Avoid unwanted construction
	Logger(const Logger&) = delete;
	// Avoid unwanted assignment
	Logger& operator=(const Logger&) = delete;

	// CSV file column headers - for results
	std::string headers; // = "Game Number,Winner,No. of Wins,Game Time,GOAP Highest Value,GOAP Highest Attack Value,GOAP Highest Defence Value,GOAP Highest Value Time Stamp,GOAP Total Builders,GOAP Total Soldiers,GOAP Total Archers,GOAP Total Walls,MCTS Highest Value,MCTS Highest Attack Value,MCTS Highest Defence Value,MCTS Highest Value Time Stamp,MCTS Total Builders,MCTS Total Soldiers,MCTS Total Archers,MCTS Total Walls,Game Over Time Stamp";

public:
	Logger();
	~Logger();

	// Log File Name
	std::string logFileName;
	// Log File Values - for results
	std::vector<std::string> results;

	// Return the Current System Time
	// @param save - is log for saving file?
	std::string GetTimeStamp(const bool save);

	// Add Event to Output Log
	// @param winner - string log of winner and game results
	void LogGame(std::string winner);
	// Save the Log File
	void SaveFile();
};

