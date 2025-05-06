#pragma once

// entt
#include "entt/entt.hpp"

#include "terrain_generator.h"

enum class EntityType
{
  // ANIMALS
  RABBIT = 0,
  BEAR,
  WOLF,
  DEER,

  // PLANTS
  BERRY
};



class EntityManager
{
public:
  EntityManager();
  ~EntityManager();

  [[nodiscard]] const entt::registry& getRegistry() const noexcept;

private:
  entt::registry registry_;
  std::unordered_map<Coordinate, std::vector<entt::entity>> entitiesMap_;
};