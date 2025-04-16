#pragma once
#include "raylib.h"

// std
#include <memory>

class Plant
{
public:
  Plant(Vector2 position, std::shared_ptr<Texture2D> texture);

  [[nodiscard]] const Vector2& getPosition() const noexcept;
  [[nodiscard]] const Texture2D& getTexture() const noexcept;

  [[nodiscard]] bool update();

private:
  Vector2 position_;
  std::shared_ptr<Texture2D> texture_;
};
