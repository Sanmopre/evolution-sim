#pragma once
#include "raylib.h"

//std
#include <string>

class Animal
{
public:
  Animal(Vector2 position, const std::string& texturePath);

  [[nodiscard]] const Vector2& getPosition() const noexcept;
  [[nodiscard]] const Texture2D& getTexture() const noexcept;

  void setNextRelativePosition(const Vector2& nextRelativePosition) noexcept;

private:
  Vector2 position_;
  Texture2D texture_;
};



