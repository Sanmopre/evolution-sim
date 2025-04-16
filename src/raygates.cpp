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
, targetFPS_(config->get()["targetFPS"].get<int>())
, expectedDeltaTime_(1.0f / static_cast<float>(targetFPS_))
, simulationSpeed_(config_->get()["simulation"]["speed"].get<int>())
  , window(windowWidth_, windowHeight_, PROJECT_NAME, targetFPS_)
{

  resourceMap_["rabbit"] = std::make_shared<Texture2D>(LoadTexture("../resources/textures/rabbit.png"));
  resourceMap_["wolf"] = std::make_shared<Texture2D>(LoadTexture("../resources/textures/wolf.png"));
  resourceMap_["herb"] = std::make_shared<Texture2D>(LoadTexture("../resources/textures/herb.png"));

  terrainGenerator_ = std::make_unique<TerrainGenerator>(mapWidth_, mapHeight_);
  terrainLoadingThread_ = std::thread(&TerrainGenerator::generate, terrainGenerator_.get());
}

Raygates::~Raygates()
{
  terrainLoadingThread_.join();
  for (const auto& [key, value] : resourceMap_)
  {
    UnloadTexture(*value);
  }
}

bool Raygates::update()
{
  if (!hasTerrainFinishedGenerating_)
  {
    if (terrainGenerator_->getLoadingProgress() >= 100.0f)
    {
      terrainLoadingThread_.join();
      terrainGenerator_->createTextureFromImage();
      initSimulation();
      hasTerrainFinishedGenerating_ = true;
      return true;
    }

    hasTerrainFinishedGenerating_ = false;
    return window.renderLoadingScreen(terrainGenerator_->getLoadingProgress());
  }

  for (int i = 0; i < simulationSpeed_; i++)
  {
    updateEntities();
  }
  return window.render(animals_, plants_, terrainGenerator_.get());
}

void Raygates::initSimulation() {
  Stats stats;
  stats.speed = config_->get()["simulation"]["rabbit"]["speed"].get<float>();
  stats.visibilityRadius =
      config_->get()["simulation"]["rabbit"]["visibilityRadius"].get<int>();

  for (int i = 0;
       i < config_->get()["simulation"]["rabbit"]["initialNumber"].get<int>();
       i++) {
    Vector2 position = getRandomWalkablePosition(terrainGenerator_.get(),
                                                 mapWidth_, mapHeight_);
    const auto rabbit = std::make_shared<Rabbit>(
        position, resourceMap_.at("rabbit"), *terrainGenerator_.get(), stats);
    animals_.emplace_back(rabbit);
  }

  for (int i = 0;
       i < config_->get()["simulation"]["herb"]["initialNumber"].get<int>();
       i++) {
    Vector2 position = getRandomPositionOfType(
        terrainGenerator_.get(), TerrainType::GRASS, mapWidth_, mapHeight_);
    const auto plant =
        std::make_shared<Plant>(position, resourceMap_.at("herb"));
    plants_.emplace_back(plant);
  }
}

void Raygates::updateEntities()
{
  for (const auto &animal : animals_)
  {
    std::ignore = animal->update(expectedDeltaTime_);
  }

  for (const auto &plant : plants_)
  {
    std::ignore = plant->update();
  }
}

} // namespace raygates
