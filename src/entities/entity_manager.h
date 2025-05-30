#pragma once

// entt
#include "entt/entt.hpp"

#include "terrain_generator.h"

enum class AnimalType {
  // ANIMALS
  RABBIT = 0,
  BEAR,
  WOLF,
  DEER
};

enum class PlantType { BERRY };

enum class State {
  IDLE,
  WANDERING,
  RUNNING,
  IN_SEARCH_FOR_RESOURCES,
  IN_SEARCH_FOR_PARTNER,
};

enum class MetricKey { HUNGER_KEY, THIRST_KEY, NEED_TO_REPRODUCE_KEY };
enum class StatsKey { SPEED, VISIBILITY_RANGE };

struct PathData {
  Path currentPath_;
  f32 tilesToMoveThisFrame_ = 0.0f;
  int currentTileIndex_ = 0;
};

struct Metric {
  Metric(f32 maxValue, f32 incrementPerUpdate)
      : value(0.0f), maxValue(maxValue),
        incrementPerUpdate(incrementPerUpdate) {}

  f32 value;
  const f32 maxValue;
  const f32 incrementPerUpdate;
};

using Metrics = std::unordered_map<MetricKey, std::shared_ptr<Metric>>;
using Stats = std::unordered_map<StatsKey, f64>;

typedef std::vector<entt::entity> EntityList;

class EntityManager {
public:
  EntityManager(const TerrainGenerator& terrainGenerator);
  ~EntityManager();

  [[nodiscard]] const entt::registry &getRegistry() const noexcept;
  void createEntity(AnimalType type, const Coordinate &coordinate,
                    const Metrics& metrics, const Stats &stats);
  void createEntity(PlantType type, const Coordinate &coordinate,
                    const Metrics& metrics, const Stats &stats);
  void updateEntities(f32 dt);

private:
  // Entity methods
  bool updateEntity(f32 dt, entt::entity entity);
  void updatePosition(f32 dt, entt::entity entity);
  [[nodiscard]] bool updateMetrics(f32 dt, entt::entity entity);
  void updateListOfInRadiusEntities(entt::entity entity);
  void setNextDestinationPosition(const Coordinate& nextDestinationPosition, entt::entity entity) noexcept;

private:
  entt::registry registry_;

private:
  const TerrainGenerator& terrainGenerator_;
  std::unordered_map<Coordinate, std::vector<entt::entity>> coordinateMap_;
};