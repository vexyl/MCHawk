// Learned from https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
#include "Logger.hpp"
#include "Utils.hpp"

#ifdef _WIN32
	#include <Windows.h>
#endif

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

// TODO: Clean this up, make some generic output with color function
void Logger::Log(LogLevel::LogLevel logLevel, const char* format, ...)
{
	char buffer[1024];

	std::va_list args;
	va_start(args, format);

	std::vsnprintf(buffer, 1024, format, args);

	va_end(args);

	bool mute = false;
	if (m_verbosityLevel < VerbosityLevel::kNormal) {
		if (logLevel == LogLevel::kDebug || logLevel == LogLevel::kWarning)
			mute = true;
	}

	std::string dateString = "--- The date is " + Utils::CurrentDate() + " ---";
	if (m_lastDateString != dateString) {
		m_logFile << dateString << "\n";

		if (!mute)
			std::cerr << dateString << "\n";

		m_lastDateString = dateString;
	}

	std::string logFileMessage = "[" + Utils::CurrentTime() + "] ";

	if (!mute)
		std::cerr << "[" + Utils::CurrentTime() + "] ";

#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD wOldColorAttrs;
	if (!mute) {
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
		wOldColorAttrs = csbiInfo.wAttributes;
	}
#endif

	if (logLevel == LogLevel::kDebug) {
		if (!mute) {
#ifdef __linux__
			std::cerr << "\033[1;32mDEBUG\033[0m ";
#elif _WIN32
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cerr << "DEBUG ";
#else
			std::cerr << "DEBUG ";
#endif
		}

		logFileMessage += "DEBUG ";
	}
	else if (logLevel == LogLevel::kInfo) {
		if (!mute) {
#ifdef __linux__
			std::cerr << "\033[0;32mINFO\033[0m ";
#elif _WIN32
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
			std::cerr << "INFO ";
#else
			std::cerr << "INFO ";
#endif
		}

		logFileMessage += "INFO ";
	}
	else if (logLevel == LogLevel::kWarning) {
		if (!mute) {
#ifdef __linux__
			std::cerr << "\033[0;31mWARNING\033[0m ";
#elif _WIN32
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			std::cerr << "WARNING ";
#else
			std::cerr << "WARNING ";
#endif
		}

		logFileMessage += "WARNING ";
	}
	else if (logLevel == LogLevel::kError) {
		if (!mute) {
#ifdef __linux__
			std::cerr << "\033[1;31mERROR\033[0m ";
#elif _WIN32
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cerr << "ERROR ";
#else
			std::cerr << "ERROR ";
#endif
		}

		logFileMessage += "ERROR ";
	}

	if (!mute) {
#ifdef _WIN32
		SetConsoleTextAttribute(hConsole, wOldColorAttrs);
#endif
		std::cout << buffer << std::endl;
	}

	logFileMessage += buffer;

	m_logFile << logFileMessage << "\n";
	m_logFile.flush();
}
