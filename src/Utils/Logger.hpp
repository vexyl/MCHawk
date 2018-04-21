#ifndef LOGGER_H
#define LOGGER_H

// Learned from https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
// TODO: Multiple log file support
// How to do this without singletons?

#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <fstream>

#define LOGGER Logger::GetLogger();
#define LOG(...) Logger::GetLogger()->Log(__VA_ARGS__);

namespace LogLevel {
	enum LogLevel { kInfo, kDebug, kError, kWarning };
}

namespace VerbosityLevel {
	enum VerbosityLevel { kQuiet, kNormal };
}

class Logger {
public:
	Logger() { m_verbosityLevel = VerbosityLevel::kNormal; }
	~Logger() { if (m_thisPtr != NULL) delete m_thisPtr; }

	static Logger* GetLogger();

	void Log(LogLevel::LogLevel logLevel, const char *format, ...);

	void SetVerbosityLevel(VerbosityLevel::VerbosityLevel verbosityLevel) { m_verbosityLevel = verbosityLevel; }

private:
	static Logger* m_thisPtr; // Singleton
	static const std::string m_logFileName;
	static std::ofstream m_logFile;

	VerbosityLevel::VerbosityLevel m_verbosityLevel;
};

#endif // LOGGER_H
