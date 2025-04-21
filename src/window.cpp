//
// Created by sanmopre on 4/9/25.
//

#include "window.h"

// raylib
#include "raygui.h"
#include "raylib.h"
#include "raymath.h"
#include "style_terminal.h"

#include <iostream>

#include <ostream>
#include <string>
#include <algorithm>

namespace raygates
{

Window::Window(int width, int height, const std::string &title, int targetFps)
  : width_(width) , height_(height)
{
  InitWindow(width, height, title.c_str());
  SetTargetFPS(targetFps);
  GuiLoadStyleTerminal();

  camera_.offset = Vector2{0, 0};
  camera_.target = Vector2{0, 0};
  camera_.zoom = 1.0f;
  camera_.rotation = 0.0f;

  shader_ = LoadShader("../resources/shaders/vertex.glsl", "../resources/shaders/fragment.glsl");
}

Window::~Window()
{
  UnloadShader(shader_);
  CloseWindow();
}

const UIState& Window::render(const std::map<u32, std::shared_ptr<Animal>>& animals, const std::map<u32, std::shared_ptr<Plant>>& plants,
                    TerrainGenerator *terrain)
{
  if (WindowShouldClose())
  {
    uiState_.windowShouldClose = true;
    return uiState_;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !sliderPressed_)
  {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f / camera_.zoom);
    camera_.target = Vector2Add(camera_.target, delta);
  }

  if (f32 wheel = GetMouseWheelMove(); wheel != 0)
  {
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

  for (const auto &[key, animal] : animals)
  {
    const auto animalPosition = animal->getPosition();

    // Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin,
    // f32 rotation, Color tint
    Rectangle source = {0, 0, static_cast<f32>(animal->getTexture().width),
                        static_cast<f32>(animal->getTexture().height)};
    Rectangle destination = {
         static_cast<f32>(animalPosition.x), static_cast<f32>(animalPosition.y),
        (static_cast<f32>(animal->getTexture().width) / 3.0f) / camera_.zoom,
        (static_cast<f32>(animal->getTexture().height) / 3.0f) /
            camera_.zoom};
    Vector2 origin = {static_cast<f32>(animal->getTexture().width) / 2.0f /
                          3.0f / camera_.zoom,
                      static_cast<f32>(animal->getTexture().height) / 2.0f /
                          3.0f / camera_.zoom};

    DrawTexturePro(animal->getTexture(), source, destination, origin, 0.0f,
                   WHITE);
  }

  for (const auto &[key, plant] : plants)
  {
    const auto plantPosition = plant->getPosition();

    // Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin,
    // f32 rotation, Color tint
    Rectangle source = {0, 0, static_cast<f32>(plant->getTexture().width),
                        static_cast<f32>(plant->getTexture().height)};
    Rectangle destination = {
        static_cast<f32>(plantPosition.x), static_cast<f32>(plantPosition.y),
        (static_cast<f32>(plant->getTexture().width) / 3.0f) / camera_.zoom,
        (static_cast<f32>(plant->getTexture().height) / 3.0f) / camera_.zoom};
    Vector2 origin = {static_cast<f32>(plant->getTexture().width) / 2.0f /
                          3.0f / camera_.zoom,
                      static_cast<f32>(plant->getTexture().height) / 2.0f /
                          3.0f / camera_.zoom};

    DrawTexturePro(plant->getTexture(), source, destination, origin, 0.0f,
                   WHITE);
  }

  EndMode2D();
  drawUI();
  EndDrawing();

  return uiState_;
}

bool Window::renderLoadingScreen(f32 value) const noexcept
{
  if (WindowShouldClose())
  {
    return false;
  }

  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
  constexpr std::string_view text = "Generating map...";
  int fontSize = 50;
  int textWidth = MeasureText(std::string(text).c_str(), fontSize);
  Vector2 progressBarSize = {width_ * 0.75f, height_ * 0.1f };
  Rectangle bounds = {(width_ - progressBarSize.x) / 2, (height_ - progressBarSize.y) / 2, progressBarSize.x, progressBarSize.y};
  GuiProgressBar(bounds, "", "", &value, 0.0, 100.0);
  DrawText("Generating map...", (width_ - textWidth) / 2, ((height_ - progressBarSize.y) / 2) - 60.0f  , fontSize, RAYWHITE);
  EndDrawing();

  return true;
}

void Window::drawPath(const Path &path) const noexcept
{
  for (const auto &[x, y] : path)
  {
    DrawPixel(x, y, WHITE);
  }
}

void Window::drawUI() noexcept
{
  GuiButton({90,0, 90, 90}, "PLAY");
  sliderPressed_ = static_cast<bool>(GuiSliderBar({180,50, 200, 20}, "", "", &uiState_.simulationSpeedSlider, 1.0f, 20.0f));
}

} // raygates