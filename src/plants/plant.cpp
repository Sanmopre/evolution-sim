#include "plant.h"

Plant::Plant(Vector2 position, const std::string &texturePath)
  : position_(position), texture_(LoadTexture(texturePath.c_str()))
{
}

const Vector2 &Plant::getPosition() const noexcept
{
  return position_;
}

const Texture2D &Plant::getTexture() const noexcept
{
  return texture_;
}

bool Plant::update()
{
  return true;
}