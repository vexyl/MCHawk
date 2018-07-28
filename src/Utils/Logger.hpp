#ifndef LOGGER_H
#define LOGGER_H

// Learned from https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
// TODO: Multiple log file support
// How to do this without singletons?

#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <fstream>

namespace LogLevel {
	enum LogLevel { kInfo, kDebug, kError, kWarning, kChat};
}

namespace VerbosityLevel {
	enum VerbosityLevel { kQuiet, kNormal };
}

class Logger {
public:
	Logger(std::string filename);

	void Log(LogLevel::LogLevel logLevel, const char *format, ...);

	void SetVerbosityLevel(VerbosityLevel::VerbosityLevel verbosityLevel) { m_verbosityLevel = verbosityLevel; }

private:
	std::ofstream m_logFile;
	std::string m_lastDateString;

	VerbosityLevel::VerbosityLevel m_verbosityLevel;
};

#endif // LOGGER_H
