#pragma once
#include "raylib.h"

// std
#include "terrain_generator.h"

#include <memory>
#include <string>


#define HUNGER_KEY "hunger"
#define THIRST_KEY "thirst"
#define NEED_TO_REPRODUCE_KEY "need_to_reproduce"



struct Stats
{
  float speed;
  int visibilityRadius;
};

struct Metric
{
  Metric(f32 maxValue, f32 incrementPerUpdate)
    :
      value(0.0f),
      maxValue(maxValue),
      incrementPerUpdate(incrementPerUpdate)
  {
  }

  f32 value;
  const f32 maxValue;
  const f32 incrementPerUpdate;
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
  Animal(u32 id, Coordinate position, std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats, std::unordered_map<Coordinate, std::vector<u32>>& coordinateMap);
  virtual ~Animal();

  [[nodiscard]] const Coordinate& getPosition() const noexcept;
  [[nodiscard]] const Texture2D& getTexture() const noexcept;

  void setNextRelativePosition(const Coordinate& nextRelativePosition) noexcept;
  void setNextDestinationPosition(const Coordinate& nextDestinationPosition) noexcept;

  [[nodiscard]] bool update(f32 dt);

private:
  void updatePosition(f32 dt);
  [[nodiscard]] bool updateMetrics(f32 dt);
  void updateListOfInRadiusEntities();

private:
  const u32 id;
  std::unordered_map<Coordinate, std::vector<u32>>& coordinateMap_;
  const TerrainGenerator& terrainGenerator_;
  Coordinate position_;
  std::shared_ptr<Texture2D> texture_;
  std::vector<u32> listOfInRadiusEntities_;

protected:
  std::map<std::string_view, std::shared_ptr<Metric>> metrics_ {};

private:
  Stats stats_;
  State state_ = State::IDLE;

private:
  Path currentPath_;
  f32 tilesToMoveThisFrame_ = 0.0f;
  int currentTileIndex_ = 0;
};



