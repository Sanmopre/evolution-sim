//
// Created by sanmopre on 4/13/25.
//

#include "animal.h"


#include <iostream>
#include <ostream>
#include <spdlog/fmt/bundled/core.h>

namespace
{

[[nodiscard]] Coordinate getRandomWalkablePosition(const TerrainGenerator& terrain, const std::vector<Coordinate>& positions)
{
  if (  const auto position =  positions.at(GetRandomValue(0, positions.size() - 1)); terrain.isWalkable(position))
  {
    return position;
  }

  return getRandomWalkablePosition(terrain, positions);
}

}

Animal::Animal(Coordinate position, std::shared_ptr<Texture2D> texture,
               const TerrainGenerator &terrainGenerator, const Stats &stats,
               std::unordered_map<Coordinate, std::vector<u32>> &coordinateMap)
    : coordinateMap_(coordinateMap), terrainGenerator_(terrainGenerator),
      position_(position), texture_(texture), stats_(stats)
{
  coordinateMap_[position_].emplace_back(2);
}

Animal::~Animal()
{
  if (coordinateMap_.find(position_) != coordinateMap_.end())
  {
    auto& vector = coordinateMap_.at(position_);
    vector.erase(std::remove(vector.begin(), vector.end(), 2), vector.end());
    if (vector.empty())
    {
      coordinateMap_.erase(position_);
    }
  }
}

const Coordinate &Animal::getPosition() const noexcept {
  return position_;
}

const Texture2D &Animal::getTexture() const noexcept {
  return *texture_;
}

void Animal::setNextRelativePosition(
    const Coordinate &nextRelativePosition) noexcept {
  position_ = Coordinate{position_.x + nextRelativePosition.x,
                      position_.y + nextRelativePosition.y};
}

void Animal::setNextDestinationPosition(
    const Coordinate &nextDestinationPosition) noexcept
{
  const auto possiblePath = terrainGenerator_.getPathToDestination(
      position_, nextDestinationPosition);

  if (possiblePath.has_value())
  {
    currentPath_ = possiblePath.value();
    state_ = State::WANDERING;
  }
}

bool Animal::update(float dt)
{
  // update coordinate map pre-move

  if (coordinateMap_.find(position_) != coordinateMap_.end())
  {
    auto& vector = coordinateMap_.at(position_);
    vector.erase(std::remove(vector.begin(), vector.end(), 2), vector.end());
    if (vector.empty())
    {
      coordinateMap_.erase(position_);
    }
  }

  // state machine
  switch (state_)
  {
  case State::IDLE:
     setNextDestinationPosition(getRandomWalkablePosition(terrainGenerator_, terrainGenerator_.getTilesInRadius(position_, stats_.visibilityRadius)));
    break;
  case State::RUNNING:
    updatePosition(dt);
    break;
  case State::WANDERING:
    updatePosition(dt);
    break;
  case State::IN_SEARCH_FOR_PARTNER:
    updatePosition(dt);
    break;
  case State::IN_SEARCH_FOR_RESOURCES:
    updatePosition(dt);
    break;
  default:
    break;
  }

  // update coordinate map post_move
  coordinateMap_[position_].emplace_back(2);

  return true;
}

void Animal::updatePosition(float dt)
{
  if (currentPath_.empty())
  {
    state_ = State::IDLE;
    return;
  }

  tilesToMoveThisFrame_ += stats_.speed * dt;
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