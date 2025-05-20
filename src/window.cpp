#include "window.h"

// raylib
#include "raygui.h"
#include "raylib.h"
#include "raymath.h"
#include "style_terminal.h"

// std
#include <iostream>
#include <ostream>
#include <string>

#define TEXTURE_SCALE 18.0f

namespace raygates {

Window::Window(int width, int height, const std::string &title, int targetFps)
    : width_(width), height_(height) {
  InitWindow(width, height, title.c_str());
  SetTargetFPS(targetFps);
  GuiLoadStyleTerminal();

  camera_.offset = Vector2{0, 0};
  camera_.target = Vector2{0, 0};
  camera_.zoom = 1.0f;
  camera_.rotation = 0.0f;

  shader_ = LoadShader("../resources/shaders/vertex.glsl",
                       "../resources/shaders/fragment.glsl");

  // Load textures
  animalResourceMap_[AnimalType::RABBIT] = std::make_shared<Texture2D>(
    LoadTexture("../resources/textures/rabbit.png"));
  animalResourceMap_[AnimalType::WOLF] = std::make_shared<Texture2D>(
      LoadTexture("../resources/textures/wolf.png"));
  animalResourceMap_[AnimalType::DEER] = std::make_shared<Texture2D>(
    LoadTexture("../resources/textures/deer.png"));
  animalResourceMap_[AnimalType::BEAR] = std::make_shared<Texture2D>(
  LoadTexture("../resources/textures/bear.png"));
  plantResourceMap_[PlantType::BERRY] = std::make_shared<Texture2D>(
      LoadTexture("../resources/textures/berries.png"));
}

Window::~Window()
{
  for (const auto &[key, value] : animalResourceMap_)
  {
    UnloadTexture(*value);
  }

  for (const auto &[key, value] : plantResourceMap_)
  {
    UnloadTexture(*value);
  }

  UnloadShader(shader_);
  CloseWindow();
}

const UIState &
Window::render(const entt::registry &registry,
               TerrainGenerator *terrain) {
  if (WindowShouldClose()) {
    uiState_.windowShouldClose = true;
    return uiState_;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !sliderPressed_) {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f / camera_.zoom);
    camera_.target = Vector2Add(camera_.target, delta);
  }

  if (f32 wheel = GetMouseWheelMove(); wheel != 0) {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera_);

    camera_.offset = GetMousePosition();
    camera_.target = mouseWorldPos;

    f32 scale = 0.2f * wheel;
    camera_.zoom = Clamp(expf(logf(camera_.zoom) + scale), 0.125f, 64.0f);
  }

  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

  BeginMode2D(camera_);
  DrawTexture(terrain->getTerrainTexture(), 0, 0, WHITE);
  BeginShaderMode(shader_);
  EndShaderMode();

  const auto animalsView = registry.view<AnimalType, Coordinate>();
  const auto plantsView = registry.view<PlantType, Coordinate>();


  for (const auto &animal : animalsView)
  {
    const auto& animalPosition = registry.get<Coordinate>(animal);
    const auto& texture = animalResourceMap_.at(registry.get<AnimalType>(animal));
    // Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin,
    // f32 rotation, Color tint
    const Rectangle source = {0, 0,
                              static_cast<f32>(texture->width),
                              static_cast<f32>(texture->height)};
    const Rectangle destination = {
        static_cast<f32>(animalPosition.x), static_cast<f32>(animalPosition.y),
        (static_cast<f32>(texture->width) / TEXTURE_SCALE) /
            camera_.zoom,
        (static_cast<f32>(texture->height) / TEXTURE_SCALE) /
            camera_.zoom};
    const Vector2 origin = {static_cast<f32>(texture->width) /
                                2.0f / TEXTURE_SCALE / camera_.zoom,
                            static_cast<f32>(texture->height) /
                                2.0f / TEXTURE_SCALE / camera_.zoom};

    DrawTexturePro(*texture, source, destination, origin, 0.0f,
                   WHITE);
  }

  for (const auto plant : plantsView)
  {
    const auto& plantPosition = registry.get<Coordinate>(plant);
    const auto& texture = plantResourceMap_.at(registry.get<PlantType>(plant));

    // Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin,
    // f32 rotation, Color tint
    const Rectangle source = {0, 0, static_cast<f32>(texture->width),
                              static_cast<f32>(texture->height)};
    const Rectangle destination = {
        static_cast<f32>(plantPosition.x), static_cast<f32>(plantPosition.y),
        (static_cast<f32>(texture->width) / TEXTURE_SCALE) /
            camera_.zoom,
        (static_cast<f32>(texture->height) / TEXTURE_SCALE) /
            camera_.zoom};
    const Vector2 origin = {static_cast<f32>(texture->width) / 2.0F /
                                TEXTURE_SCALE / camera_.zoom,
                            static_cast<f32>(texture->height) /
                                2.0F / TEXTURE_SCALE / camera_.zoom};

    DrawTexturePro(*texture, source, destination, origin, 0.0f,
                   WHITE);
  }

  EndMode2D();
  drawUI();
  EndDrawing();

  return uiState_;
}

bool Window::renderLoadingScreen(f32 value) const noexcept {
  if (WindowShouldClose()) {
    return false;
  }

  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
  constexpr std::string_view text = "Generating map...";
  int fontSize = 50;
  int textWidth = MeasureText(std::string(text).c_str(), fontSize);
  Vector2 progressBarSize = {width_ * 0.75f, height_ * 0.1f};
  Rectangle bounds = {(width_ - progressBarSize.x) / 2,
                      (height_ - progressBarSize.y) / 2, progressBarSize.x,
                      progressBarSize.y};
  GuiProgressBar(bounds, "", "", &value, 0.0, 100.0);
  DrawText("Generating map...", (width_ - textWidth) / 2,
           ((height_ - progressBarSize.y) / 2) - 60.0f, fontSize, RAYWHITE);
  EndDrawing();

  return true;
}

void Window::drawPath(const Path &path) const noexcept {
  for (const auto &[x, y] : path) {
    DrawPixel(x, y, WHITE);
  }
}

void Window::drawUI() noexcept {
  if (uiState_.simulationStop) {
    if (GuiButton({90, 0, 90, 90}, "PLAY")) {
      uiState_.simulationStop = false;
    }
  } else {
    if (GuiButton({90, 0, 90, 90}, "PAUSE")) {
      uiState_.simulationStop = true;
    }
  }
  sliderPressed_ = static_cast<bool>(
      GuiSliderBar({180, 50, 200, 20}, "", "", &uiState_.simulationSpeedSlider,
                   1.0f, 20.0f));
}

} // namespace raygates