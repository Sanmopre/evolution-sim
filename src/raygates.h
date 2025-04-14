#pragma once

#include "window.h"
#include "terrain_generator.h"

namespace raygates
{

enum class UserState
{
  UNKNOWN = 0,
  IDLE,
  PLACE_GATE,
  HOVER_NODE,
  PLACE_NODE,
};

class Raygates
{
public:
  Raygates();
  ~Raygates();

  void stateMachine();

  [[nodiscard]] bool update();

private:
  void drawUI();
  void drawGates();

private:
  Window window;
  UserState state = UserState::UNKNOWN;
};

}
