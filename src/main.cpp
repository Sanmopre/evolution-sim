#include "config.h"
#include "global.h"
#include "window.h"

// spdlog
#include "spdlog/spdlog.h"

// std
#include "evolution-sim.h"

#include <exception>
#include <memory>
#include <tuple>

auto main() -> int 
{
  try
  {
    Config config(std::string(CONFIG_PATH).append(CONFIG_FILE));
    setLogPattern();

    setLogLevel(stringToLogLevel(config.get()[LOG_LEVEL_KEY].get<std::string>()));

    spdlog::info("Starting {} version {}", PROJECT_NAME, PROJECT_VERSION);
    spdlog::debug("Build type: {}", CMAKE_BUILD_TYPE);
    spdlog::debug("CXX compiler: {}", CMAKE_CXX_COMPILER);


    auto application = std::make_unique<evo_sim::EvolutionSim>(&config);

    bool run = true;
    while (run)
    {
      run = application->update();
    }

    std::ignore = application.release();

    spdlog::info("Shutting down {}", PROJECT_NAME);
  } 
  catch (const std::exception &e)
  {
    spdlog::error("Exception caught in main: {}", e.what());
    return 1;
  }

  return 0;
}