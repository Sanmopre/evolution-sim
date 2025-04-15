#pragma once
#include "raylib.h"

// std
#include "terrain_generator.h"

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
  Animal(Vector2 position, const std::string& texturePath, const TerrainGenerator& terrainGenerator, const Stats& stats);

  [[nodiscard]] const Vector2& getPosition() const noexcept;
  [[nodiscard]] const Texture2D& getTexture() const noexcept;

  void setNextRelativePosition(const Vector2& nextRelativePosition) noexcept;
  void setNextDestinationPosition(const Vector2& nextDestinationPosition) noexcept;

  [[nodiscard]] bool update();

private:
  void updatePosition();

private:
  const TerrainGenerator& terrainGenerator_;
  Vector2 position_;
  Texture2D texture_;

private:
  Stats stats_;
  Metrics metrics_;
  State state_ = State::IDLE;

private:
  Path currentPath_;
  float tilesToMoveThisFrame_ = 0.0f;
  int currentTileIndex_ = 0;
};



