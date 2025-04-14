#pragma once

// std
#include "animals/animal.h"
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

  [[nodiscard]] bool render(const std::vector<std::shared_ptr<Animal>>& animals, TerrainGenerator* terrain);

private:
  bool showMessageBox = false;
  float currentZoomValue_ = 1.0f;
  Camera2D camera_;

private:
  Vector2 savedCurrentOffsetPoisition_;
  bool savedCurrentZoomValue_;
};

} // raygates