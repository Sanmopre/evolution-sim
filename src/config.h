// ======== config.h =======================================================================================
//
//                                     Application
//
// =========================================================================================================

#pragma once
#include <fstream>
#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"
#include "backward/backward.hpp"

#define CONFIG_FILE "config.json"
#define CONFIG_PATH "config/"

#define LOG_LEVEL_KEY "logLevel"

nlohmann::json j;

namespace backward
{

backward::SignalHandling sh;

} // namespace backward


class Config
{
public:
    static std::string getString(const std::string& key)
    {
        return j[key].get<std::string>();
    }

    static int getInt(const std::string& key)
    {
        return j[key].get<int>();
    }

    static bool getBool(const std::string& key)
    {
        return j[key].get<bool>();
    }

    static float getFloat(const std::string& key)
    {
        return j[key].get<float>();
    }
};

void loadConfig()
{
    std::string fullPath = CONFIG_PATH + std::string(CONFIG_FILE);
    std::ifstream fileStream(fullPath);
    
    if (!fileStream.is_open()) 
    {
        spdlog::error("Could not open config file at {}", fullPath);
    }
    
    try 
    {
        nlohmann::json file;
        fileStream >> file;
        j = file;
    } 
    catch (nlohmann::json::parse_error& e) 
    {
        spdlog::error("JSON parsing error: {}", e.what());
        throw;
    }
}

enum class LogLevel
{
    trace,
    debug,
    info,
    warn,
    err,
    critical,
    off
};

void setLogPattern()
{
    spdlog::set_pattern("[%T.%e] %n: %^[%l]%$ %v");
}

LogLevel stringToLogLevel(const std::string& level)
{
    if (level == "trace")
        return LogLevel::trace;
    if (level == "debug")
        return LogLevel::debug;
    if (level == "info")
        return LogLevel::info;
    if (level == "warn")
        return LogLevel::warn;
    if (level == "err")
        return LogLevel::err;
    if (level == "critical")
        return LogLevel::critical;
    if (level == "off")
        return LogLevel::off;
    return LogLevel::info;
}

void setLogLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::trace:
        spdlog::set_level(spdlog::level::trace);
        break;
    case LogLevel::debug:
        spdlog::set_level(spdlog::level::debug);
        break;
    case LogLevel::info:
        spdlog::set_level(spdlog::level::info);
        break;
    case LogLevel::warn:
        spdlog::set_level(spdlog::level::warn);
        break;
    case LogLevel::err:
        spdlog::set_level(spdlog::level::err);
        break;
    case LogLevel::critical:
        spdlog::set_level(spdlog::level::critical);
        break;
    case LogLevel::off:
        spdlog::set_level(spdlog::level::off);
        break;
    default:
        spdlog::set_level(spdlog::level::info);
        break;
    }
}