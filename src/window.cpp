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
}

Window::~Window() {
  CloseWindow();
}

bool Window::render(const std::vector<std::shared_ptr<Animal>> &animals,
                    const std::vector<std::shared_ptr<Plant>> &plants,
                    TerrainGenerator *terrain) {
  if (WindowShouldClose()) {
    return false;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f / camera_.zoom);
    camera_.target = Vector2Add(camera_.target, delta);
  }

  if (float wheel = GetMouseWheelMove(); wheel != 0)
  {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera_);

    camera_.offset = GetMousePosition();
    camera_.target = mouseWorldPos;

    float scale = 0.2f * wheel;
    camera_.zoom = Clamp(expf(logf(camera_.zoom) + scale), 0.125f, 64.0f);
  }

  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

  BeginMode2D(camera_);
  DrawTexture(terrain->getTerrainTexture(), 0, 0, WHITE);

  for (const auto &animal : animals) {
    const auto animalPosition = animal->getPosition();

    // Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin,
    // float rotation, Color tint
    Rectangle source = {0, 0, static_cast<float>(animal->getTexture().width),
                        static_cast<float>(animal->getTexture().height)};
    Rectangle destination = {
        animalPosition.x, animalPosition.y,
        (static_cast<float>(animal->getTexture().width) / 3.0f) / camera_.zoom,
        (static_cast<float>(animal->getTexture().height) / 3.0f) /
            camera_.zoom};
    Vector2 origin = {static_cast<float>(animal->getTexture().width) / 2.0f /
                          3.0f / camera_.zoom,
                      static_cast<float>(animal->getTexture().height) / 2.0f /
                          3.0f / camera_.zoom};

    DrawTexturePro(animal->getTexture(), source, destination, origin, 0.0f,
                   WHITE);
  }

  for (const auto &plant : plants) {
    const auto plantPosition = plant->getPosition();

    // Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin,
    // float rotation, Color tint
    Rectangle source = {0, 0, static_cast<float>(plant->getTexture().width),
                        static_cast<float>(plant->getTexture().height)};
    Rectangle destination = {
        plantPosition.x, plantPosition.y,
        (static_cast<float>(plant->getTexture().width) / 3.0f) / camera_.zoom,
        (static_cast<float>(plant->getTexture().height) / 3.0f) / camera_.zoom};
    Vector2 origin = {static_cast<float>(plant->getTexture().width) / 2.0f /
                          3.0f / camera_.zoom,
                      static_cast<float>(plant->getTexture().height) / 2.0f /
                          3.0f / camera_.zoom};

    DrawTexturePro(plant->getTexture(), source, destination, origin, 0.0f,
                   WHITE);
  }

  EndMode2D();
  EndDrawing();

  return true;
}

bool Window::renderLoadingScreen(float value) const noexcept
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

void Window::drawPath(const Path& path) const noexcept
{
  for (const auto &vertex : path)
  {
    DrawPixel(vertex.x, vertex.y, WHITE);
  }
}

} // raygates