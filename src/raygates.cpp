//
// Created by sanmopre on 4/10/25.
//

#include "raygates.h"

#include "animals/rabbit.h"
#include "animals/wolf.h"

#include <global.h>
#include <iostream>
#include <ostream>
#include <random>

namespace raygates
{


namespace {

[[nodiscard]] float getRandomFloat(float min = -1.0f, float max = 1.0f)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(min, max);
  return std::floor(dist(gen));
}

[[nodiscard]] Vector2 getRandomWalkablePosition(TerrainGenerator* terrain, int width, int height)
{
  if (const Vector2 position {getRandomFloat(0, width), getRandomFloat(0, height)}; terrain->isWalkable(position))
  {
    return position;
  }

  return getRandomWalkablePosition(terrain, width, height);
}

[[nodiscard]] Vector2 getRandomPositionOfType(TerrainGenerator* terrain, TerrainType type,  int width, int height)
{
  const Vector2 position {getRandomFloat(0, width), getRandomFloat(0, height)};
  if (terrain->getTerrainType(position) == type)
  {
    return position;
  }

  return getRandomPositionOfType(terrain, type, width, height);
}

}

Raygates::Raygates(Config* config) : config_(config)
, windowWidth_(config->get()["window"]["width"].get<int>())
, windowHeight_(config->get()["window"]["height"].get<int>())
, mapWidth_(config->get()["map"]["width"].get<int>())
, mapHeight_(config->get()["map"]["height"].get<int>())
  , window(windowWidth_, windowHeight_, PROJECT_NAME)
{
  terrainGenerator_ = std::make_unique<TerrainGenerator>(mapWidth_, mapHeight_);

  Stats stats;
  stats.speed = 0.2f;
  stats.visibilityRadius = 15;


  for (int i = 0; i < 70; i++)
  {
    Vector2 position = getRandomWalkablePosition(terrainGenerator_.get(), mapWidth_, mapHeight_);
    const auto rabbit = std::make_shared<Rabbit>(position, *terrainGenerator_.get(), stats);
    animals_.emplace_back(rabbit);
  }

  for (int i = 0; i < 100; i++)
  {
    Vector2 position = getRandomPositionOfType(terrainGenerator_.get(), TerrainType::GRASS ,mapWidth_, mapHeight_);
    const auto plant = std::make_shared<Plant>(position, "../resources/textures/herb.png");
    plants_.emplace_back(plant);
  }
}

Raygates::~Raygates()
{

}

bool Raygates::update()
{
  for (const auto& animal : animals_)
  {
    std::ignore = animal->update();
  }

  for (const auto& plant: plants_)
  {
    std::ignore = plant->update();
  }

  return window.render(animals_, plants_, terrainGenerator_.get());
}

} // namespace raygates
