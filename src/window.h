#pragma once

#include "entt/entt.hpp"
#include "terrain_generator.h"
#include "entities/entity_manager.h"

// std
#include <memory>
#include <string>


#define RAYGUI_IMPLEMENTATION

namespace raygates
{

struct UIState
{
  bool windowShouldClose = false;
  bool simulationStop = false;
  f32 simulationSpeedSlider = 1.0f;
};

class Window
{
public:
  Window(int width, int height, const std::string& title, int targetFps);
  ~Window();
  [[nodiscard]] const UIState& render(const entt::registry& registry, TerrainGenerator* terrain);
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

private:
  std::unordered_map<AnimalType, std::shared_ptr<Texture2D>> animalResourceMap_;
  std::unordered_map<PlantType, std::shared_ptr<Texture2D>> plantResourceMap_;
};

} // raygates