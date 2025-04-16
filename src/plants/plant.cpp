#include "plant.h"

Plant::Plant(Vector2 position, std::shared_ptr<Texture2D> texture)
  : position_(position), texture_(texture)
{
}

const Vector2 &Plant::getPosition() const noexcept
{
  return position_;
}

const Texture2D &Plant::getTexture() const noexcept
{
  return *texture_;
}

bool Plant::update()
{
  return true;
}