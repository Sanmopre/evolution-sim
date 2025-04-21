#pragma once
#include "raylib.h"

// std
#include "terrain_generator.h"

#include <memory>
#include <string>


struct Stats
{
  float speed;
  int visibilityRadius;
};

struct Metrics
{
  float hunger;
  float thirst;
  float needToReproduce;
};

enum class State
{
  IDLE,
  WANDERING,
  RUNNING,
  IN_SEARCH_FOR_RESOURCES,
  IN_SEARCH_FOR_PARTNER,
};

class Animal
{
public:
  Animal(Coordinate position, std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats);

  [[nodiscard]] const Coordinate& getPosition() const noexcept;
  [[nodiscard]] const Texture2D& getTexture() const noexcept;

  void setNextRelativePosition(const Coordinate& nextRelativePosition) noexcept;
  void setNextDestinationPosition(const Coordinate& nextDestinationPosition) noexcept;

  [[nodiscard]] bool update(float dt);

private:
  void updatePosition(float dt);

private:
  const TerrainGenerator& terrainGenerator_;
  Coordinate position_;
  std::shared_ptr<Texture2D> texture_;

private:
  Stats stats_;
  Metrics metrics_;
  State state_ = State::IDLE;

private:
  Path currentPath_;
  float tilesToMoveThisFrame_ = 0.0f;
  int currentTileIndex_ = 0;
};



