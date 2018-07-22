// Learned from https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
#include "Logger.hpp"
#include "Utils.hpp"

const std::string Logger::m_logFileName = "out.log";
Logger* Logger::m_thisPtr = nullptr;
std::ofstream Logger::m_logFile;
std::string Logger::m_lastDateString;

Logger* Logger::GetLogger()
{
	if (!m_thisPtr) {
		m_thisPtr = new Logger();
		m_logFile.open(m_logFileName.c_str(), std::ios::out | std::ios::app);
	}
	return m_thisPtr;
}

// FIXME: ANSI color codes in console probably isn't a portable way to do this
// FIXME: Should only show date in log if nothing has been logged that day
void Logger::Log(LogLevel::LogLevel logLevel, const char* format, ...)
{
	char buffer[1024];

	va_list args;
	va_start(args, format);

	vsnprintf(buffer, 1024, format, args);

	va_end(args);

	std::string dateString = Utils::CurrentDate();
	if (m_lastDateString != dateString) {
		m_logFile << "--- " << dateString << " ---\n";
		std::cerr << "--- " << dateString << " ---" << std::endl;
		m_lastDateString = dateString;
	}

	std::string consoleMessage = "[" + Utils::CurrentTime() + "] ";
	std::string logFileMessage = "[" + Utils::CurrentTime() + "] ";
	if (logLevel == LogLevel::kDebug) {
		consoleMessage += "\033[1;32mDEBUG\033[0m ";
		logFileMessage += "DEBUG ";
	} else if (logLevel == LogLevel::kInfo) {
		consoleMessage += "\033[0;32mINFO\033[0m ";
		logFileMessage += "INFO ";
	} else if (logLevel == LogLevel::kWarning) {
		consoleMessage += "\033[0;31mWARNING\033[0m ";
		logFileMessage += "WARNING ";
	} else if (logLevel == LogLevel::kError) {
		consoleMessage += "\033[1;31mERROR\033[0m ";
		logFileMessage += "ERROR ";
	}

	consoleMessage += buffer;
	logFileMessage += buffer;

	m_logFile << logFileMessage << "\n";
	m_logFile.flush();

	if (m_verbosityLevel < VerbosityLevel::kNormal) {
		if (logLevel == LogLevel::kDebug || logLevel == LogLevel::kWarning)
			return;
	}

	std::cerr << consoleMessage << std::endl;
}
