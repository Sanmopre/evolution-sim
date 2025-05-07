#include "entity_manager.h"

EntityManager::EntityManager() {}

EntityManager::~EntityManager() { registry_.clear(); }

const entt::registry &EntityManager::getRegistry() const noexcept {
  return registry_;
}

void EntityManager::createEntity(AnimalType type, const Coordinate &coordinate,
                                 const Metrics &metrics, const Stats &stats) {
  const auto entity = registry_.create();

  // Set values
  registry_.emplace<AnimalType>(entity, type);
  registry_.emplace<Coordinate>(entity, coordinate);
  registry_.emplace<Metrics>(entity, metrics);
  registry_.emplace<Stats>(entity, stats);

  // Init non arg properties
  registry_.emplace<PathData>(entity);
}

void EntityManager::createEntity(PlantType type, const Coordinate &coordinate,
                                 const Metrics &metrics, const Stats &stats) {
  const auto entity = registry_.create();

  // Set values
  registry_.emplace<PlantType>(entity, type);
  registry_.emplace<Coordinate>(entity, coordinate);
  registry_.emplace<Metrics>(entity, metrics);
  registry_.emplace<Stats>(entity, stats);
}

void EntityManager::updateEntities()
{
  const auto view = registry_.view<AnimalType, Coordinate, Metrics, Stats>();

  for (const auto &entity : view) {
    const Coordinate &coordinate = view.get<Coordinate>(entity);

    entitiesMap_[coordinate].emplace_back(entity);
  }
}