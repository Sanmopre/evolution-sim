//
// Created by sanmopre on 4/13/25.
//

#include "animal.h"


#include <iostream>
#include <ostream>
#include <spdlog/fmt/bundled/core.h>

namespace
{

[[nodiscard]] Vector2 getRandomWalkablePosition(const TerrainGenerator& terrain, const std::vector<Vector2>& positions)
{
  if (  const auto position =  positions.at(GetRandomValue(0, positions.size() - 1)); terrain.isWalkable(position))
  {
    return position;
  }

  return getRandomWalkablePosition(terrain, positions);
}

}

Animal::Animal(Vector2 position, const std::string& texturePath, const TerrainGenerator& terrainGenerator, const Stats& stats)
  :
    terrainGenerator_(terrainGenerator)
  , position_(position)
  , texture_(LoadTexture(texturePath.c_str()))
  , stats_(stats)
{

}

const Vector2 &Animal::getPosition() const noexcept {
  return position_;
}

const Texture2D &Animal::getTexture() const noexcept {
  return texture_;
}

void Animal::setNextRelativePosition(
    const Vector2 &nextRelativePosition) noexcept {
  position_ = Vector2{position_.x + nextRelativePosition.x,
                      position_.y + nextRelativePosition.y};
}

void Animal::setNextDestinationPosition(
    const Vector2 &nextDestinationPosition) noexcept
{
  const auto possiblePath = terrainGenerator_.getPathToDestination(
      position_, nextDestinationPosition);

  if (possiblePath.has_value())
  {
    currentPath_ = possiblePath.value();
    state_ = State::WANDERING;
  }
}

bool Animal::update()
{
  switch (state_)
  {
  case State::IDLE:
     setNextDestinationPosition(getRandomWalkablePosition(terrainGenerator_, terrainGenerator_.getTilesInRadius(position_, stats_.visibilityRadius)));
    break;
  case State::RUNNING:
    updatePosition();
    break;
  case State::WANDERING:
    updatePosition();
    break;
  case State::IN_SEARCH_FOR_PARTNER:
    updatePosition();
    break;
  case State::IN_SEARCH_FOR_RESOURCES:
    updatePosition();
    break;
  default:
    break;
  }

  return true;
}

void Animal::updatePosition()
{
  if (currentPath_.empty())
  {
    state_ = State::IDLE;
    return;
  }

  tilesToMoveThisFrame_ += stats_.speed;
  const auto tilesToMove = static_cast<int>(tilesToMoveThisFrame_);

  if (tilesToMove > 0)
  {
    tilesToMoveThisFrame_ -= static_cast<float>(tilesToMove);
  }
  else
  {
    return;
  }

  currentTileIndex_ += tilesToMove;

  if (currentTileIndex_ >= currentPath_.size())
  {
    position_ = currentPath_.back();
    currentPath_.clear();
    currentTileIndex_ = 0;
    state_ = State::IDLE;
  }
  else
  {
    position_ = currentPath_[currentTileIndex_ - 1];
  }
}