// ======== config.h =======================================================================================
//
//                                     Application
//
// =========================================================================================================

#pragma once
#include <fstream>
#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

#define CONFIG_FILE "config.json"
#define CONFIG_PATH "../config/"

#define LOG_LEVEL_KEY "logLevel"

class Config
{
public:
  explicit Config(const std::string &configPath);

  [[nodiscard]] const nlohmann::json &get() const noexcept
  {
    return j;
  }

private:
  nlohmann::json j;
};

inline Config::Config(const std::string &configPath)
{
  std::ifstream fileStream(configPath);

  if (!fileStream.is_open())
  {
    spdlog::error("Could not open config file at {}", configPath);
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

inline void setLogPattern()
{
    spdlog::set_pattern("[%T.%e] %n: %^[%l]%$ %v");
}

inline LogLevel stringToLogLevel(const std::string& level)
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

inline void setLogLevel(LogLevel level)
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