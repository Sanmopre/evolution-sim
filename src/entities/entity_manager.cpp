#include "entity_manager.h"


const std::vector<MetricKey> deadlyMetrics {MetricKey::HUNGER_KEY, MetricKey::THIRST_KEY};

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

EntityManager::EntityManager(const TerrainGenerator& terrainGenerator) : terrainGenerator_(terrainGenerator) {}

EntityManager::~EntityManager() { registry_.clear(); }

const entt::registry &EntityManager::getRegistry() const noexcept {
  return registry_;
}

void EntityManager::createEntity(AnimalType type, const Coordinate &coordinate,
                                const Metrics& metrics, const Stats &stats)
{
  const auto entity = registry_.create();

  // Set values
  registry_.emplace<AnimalType>(entity, type);
  registry_.emplace<Coordinate>(entity, coordinate);
  registry_.emplace<Metrics>(entity, metrics);
  registry_.emplace<Stats>(entity, stats);
  registry_.emplace<State>(entity, State::IDLE);
  registry_.emplace<EntityList>(entity);

  // Init non arg properties
  registry_.emplace<PathData>(entity);
}

void EntityManager::createEntity(PlantType type, const Coordinate &coordinate,
                                 const Metrics& metrics, const Stats &stats)
{
  const auto entity = registry_.create();

  // Set values
  registry_.emplace<PlantType>(entity, type);
  registry_.emplace<Coordinate>(entity, coordinate);
  registry_.emplace<Metrics>(entity, metrics);
  registry_.emplace<Stats>(entity, stats);
}

void EntityManager::updateEntities(f32 dt)
{
  const auto view = registry_.view<AnimalType, Coordinate, Metrics, Stats>();

  std::vector<entt::entity> entitiesToDelete_;
  entitiesToDelete_.reserve(registry_.view<Coordinate>().size());
  for (const auto &entity : view)
  {
    updateEntity(dt, entity);
  }

  for (const auto &entity : entitiesToDelete_)
  {
    registry_.destroy(entity);
    const Coordinate &coordinate = view.get<Coordinate>(entity);
    std::ignore = coordinate;
    // TODO: Remoce the entity from the coordinate map
  }

}

bool EntityManager::updateEntity(f32 dt, entt::entity entity)
{

  switch (registry_.get<State>(entity))
  {
  case State::IDLE:
    {
    const auto& tiles = terrainGenerator_.getTilesInRadius(
                               registry_.get<Coordinate>(entity), registry_.get<Stats>(entity).at(StatsKey::VISIBILITY_RANGE));
    setNextDestinationPosition(getRandomWalkablePosition(
        terrainGenerator_, tiles), entity);
    //updateListOfInRadiusEntities(entity);
  }
    break;
  case State::IN_SEARCH_FOR_PARTNER:
    updatePosition(dt, entity);
    break;
  case State::IN_SEARCH_FOR_RESOURCES:
    updatePosition(dt, entity);
    break;
  case State::RUNNING:
    updatePosition(dt, entity);
    break;
  case State::WANDERING:
    updatePosition(dt, entity);
    break;
  }

  // update coordinate map post_move
  // TODO:
  //coordinateMap_[registry_.get<Coordinate>(entity)].emplace_back(entity);

  return updateMetrics(dt, entity);
}

void EntityManager::updatePosition(f32 dt, entt::entity entity) {
  auto &pathData = registry_.get<PathData>(entity);
  auto &state = registry_.get<State>(entity);
  auto &position = registry_.get<Coordinate>(entity);
  const auto &stats = registry_.get<Stats>(entity);

  if (pathData.currentPath_.empty()) {
    state = State::IDLE;
    return;
  }

  pathData.tilesToMoveThisFrame_ += stats.at(StatsKey::SPEED) * dt;
  const auto tilesToMove = static_cast<int>(pathData.tilesToMoveThisFrame_);

  if (tilesToMove > 0) {
    pathData.tilesToMoveThisFrame_ -= static_cast<float>(tilesToMove);
  } else {
    return;
  }

  pathData.currentTileIndex_ += tilesToMove;

  if (pathData.currentTileIndex_ >= pathData.currentPath_.size()) {
    position = pathData.currentPath_.back();
    pathData.currentPath_.clear();
    pathData.currentTileIndex_ = 0;
    state = State::IDLE;
  } else {
    position = pathData.currentPath_[pathData.currentTileIndex_ - 1];
  }
}

bool EntityManager::updateMetrics(f32 dt, entt::entity entity)
{
  for (auto& [key, value] : registry_.get<Metrics>(entity))
  {
    value->value = value->value + value->incrementPerUpdate * dt;
    if (value->value >= value->maxValue)
    {
      if (std::find(deadlyMetrics.begin(), deadlyMetrics.end(), key) != deadlyMetrics.end())
      {
        return false;
      }
    }
  }
  return true;
}


void EntityManager::updateListOfInRadiusEntities(entt::entity entity)
{
  auto& list = registry_.get<EntityList>(entity);
  list.clear();

  const auto& visibleTiles = terrainGenerator_.getTilesInRadius(registry_.get<Coordinate>(entity), registry_.get<Stats>(entity).at(StatsKey::VISIBILITY_RANGE));

  for (const auto& tile : visibleTiles)
  {
    if (terrainGenerator_.isWalkable(tile))
    {
      if (coordinateMap_.find(tile) != coordinateMap_.end())
      {
        for (const auto& entityId : coordinateMap_.at(tile))
        {
          list.emplace_back(entityId);
        }
      }
    }
  }
}

void EntityManager::setNextDestinationPosition(
    const Coordinate &nextDestinationPosition, entt::entity entity) noexcept
{
  const auto possiblePath = terrainGenerator_.getPathToDestination(
    registry_.get<Coordinate>(entity), nextDestinationPosition);

  if (possiblePath.has_value())
  {
    registry_.get<PathData>(entity).currentPath_ = possiblePath.value();
    registry_.get<State>(entity) = State::WANDERING;
  }
}