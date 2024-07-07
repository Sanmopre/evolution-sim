#include "config.h"
#include "global.h"


auto main() -> int 
{
    try {
        loadConfig();
        setLogPattern();

        setLogLevel(stringToLogLevel(Config::getString(LOG_LEVEL_KEY)));
        
        spdlog::info("Starting {} version {}", PROJECT_NAME, PROJECT_VERSION);
        spdlog::debug("Build type: {}", CMAKE_BUILD_TYPE);
        spdlog::debug("CXX compiler: {}", CMAKE_CXX_COMPILER);
    } 
    catch (const std::exception& e) 
    {
        spdlog::error("Exception caught in main: {}", e.what());
        return 1;
    }

    return 0;
}