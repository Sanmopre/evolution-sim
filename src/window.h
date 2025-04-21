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

struct UIState
{
  bool windowShouldClose = false;
  f32 simulationSpeedSlider = 1.0f;
};

class Window
{
public:
  Window(int width, int height, const std::string& title, int targetFps);
  ~Window();
  [[nodiscard]] const UIState& render(const std::map<u32, std::shared_ptr<Animal>>& animals, const std::map<u32, std::shared_ptr<Plant>>& plants, TerrainGenerator* terrain);
  [[nodiscard]] bool renderLoadingScreen(float value) const noexcept;
private:
  void drawPath(const Path& path)const noexcept;
  void drawUI() noexcept;

private:
  bool showMessageBox = false;
  float currentZoomValue_ = 1.0f;
  Camera2D camera_;
  Shader shader_;

private:
  int width_;
  int height_;

private:
  bool sliderPressed_ = false;
  UIState uiState_;
};

} // raygates