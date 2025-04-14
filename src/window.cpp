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

  terrainGenerator_ = std::make_unique<TerrainGenerator>(1920, 1080);

  camera_.offset = Vector2{0, 0};
  camera_.target = Vector2{0, 0};
  camera_.zoom = 1.0f;
  camera_.rotation = 0.0f;
}

Window::~Window() {
  CloseWindow();
}

auto Window::update() -> bool
{
  if (WindowShouldClose()) {
    return false;
  }

  const auto mouseWheel = GetMouseWheelMove();

  currentZoomValue_ += mouseWheel;
  currentZoomValue_ = std::clamp(currentZoomValue_, 1.0f, 20.0f);
  camera_.zoom = exponentialScale(currentZoomValue_);

  camera_.offset = Vector2{GetMousePosition().x * camera_.zoom,GetMousePosition().y* camera_.zoom};
  camera_.target = Vector2{GetMousePosition().x* camera_.zoom,GetMousePosition().y* camera_.zoom};

  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

  BeginMode2D(camera_);
  DrawTexture(terrainGenerator_->getTerrainTexture(), 0, 0, WHITE);
  EndMode2D();

  if (GuiButton((Rectangle){24, 24, 120, 30}, "#191#Show Message") != 0)
  {
    showMessageBox = true;
  }


  GuiButton((Rectangle){160, 900, 200, 100}, "AND");
  GuiButton((Rectangle){360, 900, 200, 100}, "NAND");
  GuiButton((Rectangle){560, 900, 200, 100}, "OR");
  GuiButton((Rectangle){760, 900, 200, 100}, "NOR");

  GuiButton((Rectangle){960, 900, 200, 100}, "BUFFER");
  GuiButton((Rectangle){1160, 900, 200, 100}, "NOT");
  GuiButton((Rectangle){1360, 900, 200, 100}, "XOR");
  GuiButton((Rectangle){1560, 900, 200, 100}, "XNOR");

  if (showMessageBox)
  {
    int const result =
        GuiMessageBox((Rectangle){85, 70, 250, 100}, "#191#Message Box",
                      "Hi! This is a message!", "");

    if (result >= 0) {
      showMessageBox = false;
    }
  }

  EndDrawing();

  return true;
}

} // raygates