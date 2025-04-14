//
// Created by sanmopre on 4/13/25.
//

#include "animal.h"

Animal::Animal(Vector2 position, const std::string& texturePath)
  : position_(position)
  , texture_(LoadTexture(texturePath.c_str()))
{

}

const Vector2 &Animal::getPosition() const noexcept {
  return position_;
}

const Texture2D &Animal::getTexture() const noexcept {
  return texture_;
}

void Animal::setNextRelativePosition(
    const Vector2 &nextRelativePosition) noexcept
{
  position_ = Vector2{position_.x + nextRelativePosition.x, position_.y + nextRelativePosition.y};
}