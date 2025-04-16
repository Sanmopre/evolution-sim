#pragma once

// std
#include "animals/animal.h"
#include "plants/plant.h"
#include "terrain_generator.h"

#include <memory>
#include <string>


#define RAYGUI_IMPLEMENTATION

namespace raygates
{

class Window
{
public:
  Window(int width, int height, const std::string& title, int targetFps);
  ~Window();

  [[nodiscard]] bool render(const std::vector<std::shared_ptr<Animal>>& animals, const std::vector<std::shared_ptr<Plant>>& plants, TerrainGenerator* terrain);
  [[nodiscard]] bool renderLoadingScreen(float value) const noexcept;
private:
  void drawPath(const Path& path)const noexcept;

private:
  bool showMessageBox = false;
  float currentZoomValue_ = 1.0f;
  Camera2D camera_;
private:
  int width_;
  int height_;
};

} // raygates