#include "entity_manager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager() {}

const entt::registry &EntityManager::getRegistry() const noexcept
{
  return registry_;
}