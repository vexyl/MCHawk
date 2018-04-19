// Learned from https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
#include "Logger.hpp"
#include "Utils.hpp"

const std::string Logger::m_logFileName = "out.log";
Logger* Logger::m_thisPtr = NULL;
std::ofstream Logger::m_logFile;

Logger* Logger::GetLogger()
{
	if (!m_thisPtr) {
		m_thisPtr = new Logger();
		m_logFile.open(m_logFileName.c_str(), std::ios::out | std::ios::app);
	}
	return m_thisPtr;
}

void Logger::Log(LogLevel::LogLevel logLevel, const char *format, ...)
{
	char buffer[1024];

	va_list args;
	va_start(args, format);

	vsnprintf(buffer, 1024, format, args);

	va_end(args);

	std::string message;
	message = "[" + Utils::CurrentDateTime() + "] ";
	if (logLevel == LogLevel::kDebug)
		message += "DEBUG: ";
	else if (logLevel == LogLevel::kWarning)
		message += "WARNING: ";
	else if (logLevel == LogLevel::kError)
		message += "ERROR: ";
	message += buffer;

	m_logFile << message << "\n";
	m_logFile.flush();

	if (m_verbosityLevel < VerbosityLevel::kNormal) {
		if (logLevel == LogLevel::kDebug || logLevel == LogLevel::kWarning)
			return;
	}

	std::cerr << message << std::endl;
}
