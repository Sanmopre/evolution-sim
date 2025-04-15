#pragma once
#include "raylib.h"

// std
#include <string>

class Plant
{
public:
  Plant(Vector2 position, const std::string& texturePath);

  [[nodiscard]] const Vector2& getPosition() const noexcept;
  [[nodiscard]] const Texture2D& getTexture() const noexcept;

  [[nodiscard]] bool update();

private:
  Vector2 position_;
  Texture2D texture_;
};
