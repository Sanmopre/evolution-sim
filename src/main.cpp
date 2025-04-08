#include "config.h"
#include "global.h"
#include "spdlog/spdlog.h"
#include <exception>

#include "raylib.h"

auto main() -> int 
{
  try {
    loadConfig();
    setLogPattern();

    setLogLevel(stringToLogLevel(Config::getString(LOG_LEVEL_KEY)));

    spdlog::info("Starting {} version {}", PROJECT_NAME, PROJECT_VERSION);
    spdlog::debug("Build type: {}", CMAKE_BUILD_TYPE);
    spdlog::debug("CXX compiler: {}", CMAKE_CXX_COMPILER);

    // Initialize the window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "My First Raylib Window");

    // Set the frame rate
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
      // Start drawing
      BeginDrawing();

      ClearBackground(RAYWHITE);
      DrawText("Hello, Raylib!", 350, 200, 20, LIGHTGRAY);

      EndDrawing();
    }

    // Clean up
    CloseWindow();

    // Start your application here

    spdlog::info("Shutting down {}", PROJECT_NAME);
  } 
  catch (const std::exception &e)
  {
    spdlog::error("Exception caught in main: {}", e.what());
    return 1;
  }

  return 0;
}