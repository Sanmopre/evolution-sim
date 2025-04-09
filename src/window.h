#pragma once

//std
#include <string>

#define RAYGUI_IMPLEMENTATION

namespace raygates
{

class Window
{
public:
  Window(int width, int height, const std::string& title);
  ~Window();

  [[nodiscard]] bool update();

private:
  bool showMessageBox = false;
};

} // raygates