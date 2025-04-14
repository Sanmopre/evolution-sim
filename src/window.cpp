//
// Created by sanmopre on 4/9/25.
//

#include "window.h"

// raylib
#include "raygui.h"
#include "raylib.h"
#include "style_terminal.h"

#include <iostream>

#include <ostream>
#include <string>
#include <algorithm>

namespace raygates
{


namespace
{

float exponentialScale(float x) {
  // Normalize to [0, 1]
  float normalized = (x - 1.0f) / (20.0f - 1.0f);

  // Apply power curve — try exponent > 1 for slow start
  float curved = std::pow(normalized, 2.0f); // square it

  // Map back to [1, 20]
  return 1.0f + curved * (20.0f - 1.0f);
}

}


Window::Window(int width, int height, const std::string &title)
{
  InitWindow(width, height, title.c_str());
  SetTargetFPS(60);
  GuiLoadStyleTerminal();

  camera_.offset = Vector2{0, 0};
  camera_.target = Vector2{0, 0};
  camera_.zoom = 1.0f;
  camera_.rotation = 0.0f;
}

Window::~Window() {
  CloseWindow();
}

bool Window::render(const std::vector<std::shared_ptr<Animal>>& animals, TerrainGenerator* terrain)
{
  if (WindowShouldClose())
  {
    return false;
  }

  const auto mouseWheel = GetMouseWheelMove();

  currentZoomValue_ += mouseWheel;
  currentZoomValue_ = std::clamp(currentZoomValue_, 1.0f, 20.0f);
  camera_.zoom = exponentialScale(currentZoomValue_);

  camera_.target = GetScreenToWorld2D(GetMousePosition(), camera_);
  camera_.offset = Vector2{GetMousePosition().x ,GetMousePosition().y};


  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

  BeginMode2D(camera_);
  DrawTexture(terrain->getTerrainTexture(), 0, 0, WHITE);

  for (const auto& animal : animals)
  {
    const auto animalPosition = animal->getPosition();

    // Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint
    Rectangle source = {0 ,0 , static_cast<float>(animal->getTexture().width), static_cast<float>(animal->getTexture().height)};
    Rectangle destination = {animalPosition.x, animalPosition.y, (static_cast<float>(animal->getTexture().width) / 3.0f) / camera_.zoom, (static_cast<float>(animal->getTexture().height)/ 3.0f) / camera_.zoom};
    DrawTexturePro(animal->getTexture(), source,destination, Vector2{}, 0.0f, WHITE);
  }

  EndMode2D();
  EndDrawing();

  return true;
}

} // raygates