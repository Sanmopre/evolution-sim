#pragma once

// std
#include "terrain_generator.h"

#include <memory>
#include <string>


#define RAYGUI_IMPLEMENTATION

namespace raygates
{

class Window
{
public:
  Window(int width, int height, const std::string& title);
  ~Window();

  [[nodiscard]] bool update();

private:
  bool showMessageBox = false;
  float currentZoomValue_ = 1.0f;
  std::unique_ptr<TerrainGenerator> terrainGenerator_;
  Camera2D camera_;

};

} // raygates