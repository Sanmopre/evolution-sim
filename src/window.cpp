//
// Created by sanmopre on 4/9/25.
//

#include "window.h"

// raylib
#include "raygui.h"
#include "raylib.h"
#include "style_terminal.h"
#include <string>

namespace raygates
{

Window::Window(int width, int height, const std::string &title)
{
  InitWindow(width, height, title.c_str());
  SetTargetFPS(60);
  GuiLoadStyleTerminal();
}

Window::~Window() {
  CloseWindow();
}
auto Window::update() -> bool {
  if (WindowShouldClose()) {
    return false;
  }

  BeginDrawing();
  ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

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