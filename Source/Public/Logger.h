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
    Initialization,
    Shader_Initialization,
    Engine
};

class Logger {

    unsigned int LogLevel : 8;


public:

    Logger()
    {
        LogLevel = LOG_TYPE_LEVEL_ALL;
    }

    Logger(unsigned int InLogLevel)
    {
        LogLevel = InLogLevel;
    }

    ~Logger() {}

private:

    std::string GetLogTypeAsString(unsigned int LogType, bool NoColor = false) {
        if (!NoColor)
        {
            switch (LogType)
            {
                case LOG_TYPE_LEVEL_ALL:
                    return "[ALL]";
                case LOG_TYPE_LEVEL_ERROR:
                    return std::string(RED) + "[ERROR]" + RESET;
                case LOG_TYPE_LEVEL_INFO:
                    return std::string(GREEN) + "[INFO]" + RESET;
                case LOG_TYPE_LEVEL_WARNING:
                    return std::string(YELLOW) + "[WARNING]" + RESET;
                case LOG_TYPE_LEVEL_DEBUG:
                    return std::string(BLUE) + "[DEBUG]" + RESET;

                default:
                    return std::string(BOLDWHITE) + "[*]" + RESET;
            }
        }
        else
        {
            switch (LogType) {
                case LOG_TYPE_LEVEL_ALL:
                    return "[ALL]";
                case LOG_TYPE_LEVEL_ERROR:
                    return "[ERROR]";
                case LOG_TYPE_LEVEL_INFO:
                    return "[INFO]";
                case LOG_TYPE_LEVEL_WARNING:
                    return "[WARNING]";
                case LOG_TYPE_LEVEL_DEBUG:
                    return "[DEBUG]";

                default:
                    return "[*]";
            }
        }

    }

    std::string GetLogCategoryAsString(LogCategory LogCategory) {
        switch (LogCategory) {
            case LogCategory::None:
                return "(None)";
            case LogCategory::Engine:
                return "(Engine)";
            case LogCategory::Initialization:
                return "(Initialization)";
            case LogCategory::Shader_Initialization:
                return "(Shader_Initialization)";

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

    bool LogToFile(std::string MessageToLog) {
        std::ofstream file;
        file.open("phantom.log", std::ios_base::app);

        if (!file.is_open())
        {
            return false;
        }

        file << MessageToLog << std::endl;
        file.close();

        return true;
    }

public:

    bool Log(std::string MessageToLog, unsigned int LogType, LogCategory LogCategory = LogCategory::None) {
        if ((LogLevel & LogType) == 0)
            return false;

        std::string newString = "";
        newString += GetLogTypeAsString(LogType) + " - ";
        newString += GetLogCategoryAsString(LogCategory) + ": ";
        newString += MessageToLog;

        std::cout << newString << std::endl;

        newString = "";
        newString += GetLogTypeAsString(LogType, true) + " - ";
        newString += GetLogCategoryAsString(LogCategory) + ": ";
        newString += MessageToLog;


        return LogToFile(newString);
    }

};