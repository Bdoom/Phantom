//
// Created by decil on 4/14/21.
//

#ifndef PHANTOM_LOGGER_H
#define PHANTOM_LOGGER_H

#endif //PHANTOM_LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <time.h>
#include <ctime>

using namespace std;


#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define LOG_TYPE_LEVEL_ERROR 0b0000'0001
#define LOG_TYPE_LEVEL_INFO 0b0000'0010
#define LOG_TYPE_LEVEL_WARNING 0b0000'0100
#define LOG_TYPE_LEVEL_DEBUG 0b0000'1000
#define LOG_TYPE_LEVEL_ALL 0b1111'1111

enum class LogCategory {
    None,
    Engine
};

class Logger {

    unsigned int LogLevel : 5;


public:
    Logger()
    {

    }

    Logger(unsigned int InLogLevel)
    {
        LogLevel = InLogLevel;
    }

private:

    string GetLogTypeAsString(unsigned int LogType) {
        switch (LogType) {
            case LOG_TYPE_LEVEL_ALL:
                return "[ALL]";
            case LOG_TYPE_LEVEL_ERROR:
                return string(RED) + "[ERROR]" + RESET;
            case LOG_TYPE_LEVEL_INFO:
                return string(GREEN) + "[INFO]" + RESET;
            case LOG_TYPE_LEVEL_WARNING:
                return string(YELLOW) + "[WARNING]" + RESET;
            case LOG_TYPE_LEVEL_DEBUG:
                return string(BLUE) + "[DEBUG]" + RESET;

            default:
                return string(BOLDWHITE) + "[*]" + RESET;
        }
    }

    string GetTime(void) {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct tm timeinfo;
#ifdef _WIN32
        ::localtime_s(&timeinfo, &now);
#elif _WIN64
        ::localtime_s(&timeinfo, &now);
#else
        localtime(&now);
#endif
        char buffer[32];
        std::strftime(buffer, 32, "%m/%d/%Y - %H:%M:%S", &timeinfo);

        return buffer;
    }

    string GetLogCategoryAsString(LogCategory LogCategory) {
        switch (LogCategory) {
            case LogCategory::None:
                return "(None)";
            case LogCategory::Engine:
                return "(Engine)";

            default:
                return "(*)";
        }
    }

public:
    void SetLogLevel(unsigned int NewLogLevel) {
        LogLevel = NewLogLevel;
    }

    void AddLogLevel(unsigned int LogLevelToAdd) {
        LogLevel |= LogLevelToAdd;
    }

    void RemoveLogLevel(unsigned int LogLevelToRemove) {
        LogLevel &= ~LogLevelToRemove;
    }

private:

    void LogToFile(string MessageToLog) {
        ofstream file;
        file.open("phantom.log", ios_base::app);
        file << MessageToLog << endl;
        file.close();
    }

public:

    void Log(string MessageToLog, unsigned int LogType, LogCategory LogCategory) {
        if ((LogLevel & LogType) == 0)
            return;

        string newString = "";
        newString += GetLogTypeAsString(LogType) + " - ";
        newString += GetLogCategoryAsString(LogCategory) + ": ";
        newString += MessageToLog;

        cout << newString << endl;
        LogToFile(MessageToLog);
    }

};