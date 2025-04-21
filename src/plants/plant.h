#pragma once
#include "raylib.h"

// std
#include <memory>

#include "config.h"

class Plant
{
public:
  Plant(Coordinate position, std::shared_ptr<Texture2D> texture);

  [[nodiscard]] const Coordinate& getPosition() const noexcept;
  [[nodiscard]] const Texture2D& getTexture() const noexcept;

  [[nodiscard]] bool update();

private:
  Coordinate position_;
  std::shared_ptr<Texture2D> texture_;
};
