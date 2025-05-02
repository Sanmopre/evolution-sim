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

std::unordered_map<Coordinate, std::vector<u32>> coordinateMap_;

namespace
{

[[nodiscard]] u32 generateId()
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<u32> dist;
  return dist(gen);
}

[[nodiscard]] Coordinate getRandomWalkablePosition(TerrainGenerator* terrain, u16 width, u16 height)
{
  if (const Coordinate position {GetRandomValue(0, width), GetRandomValue(0, height)}; terrain->isWalkable(position))
  {
    return position;
  }

  return getRandomWalkablePosition(terrain, width, height);
}

[[nodiscard]] Coordinate getRandomPositionOfType(TerrainGenerator* terrain, TerrainType type,  u16 width, u16 height)
{
  if (  const Coordinate position {GetRandomValue(0, width - 1), GetRandomValue(0, height - 1)}; terrain->getTerrainType(position) == type)
  {
    return position;
  }

  return getRandomPositionOfType(terrain, type, width, height);
}

}

Raygates::Raygates(Config *config)
    : config_(config),
      windowWidth_(config->get()["window"]["width"].get<u16>()),
      windowHeight_(config->get()["window"]["height"].get<u16>()),
      mapWidth_(config->get()["map"]["width"].get<u16>()),
      mapHeight_(config->get()["map"]["height"].get<u16>()),
      targetFPS_(config->get()["targetFPS"].get<u16>()),
      expectedDeltaTime_(1.0f / static_cast<f32>(targetFPS_)),
      window(windowWidth_, windowHeight_, PROJECT_NAME, targetFPS_),
      coordinateMap_() {

  resourceMap_["rabbit"] = std::make_shared<Texture2D>(
      LoadTexture("../resources/textures/rabbit.png"));
  resourceMap_["wolf"] = std::make_shared<Texture2D>(
      LoadTexture("../resources/textures/wolf.png"));
  resourceMap_["berries"] = std::make_shared<Texture2D>(
      LoadTexture("../resources/textures/berries.png"));

  terrainGenerator_ = std::make_unique<TerrainGenerator>(mapWidth_, mapHeight_);
  terrainLoadingThread_ =
      std::thread(&TerrainGenerator::generate, terrainGenerator_.get());
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

  if (!uiState_.simulationStop)
  {
    for (u8 i = 0; i < static_cast<u8>(uiState_.simulationSpeedSlider); i++)
    {
      updateEntities();
    }
  }

  uiState_ = window.render(animals_, plants_, terrainGenerator_.get());

  return !uiState_.windowShouldClose;
}

void Raygates::initSimulation()
{
  Stats stats;
  stats.speed = config_->get()["simulation"]["rabbit"]["speed"].get<f32>();
  stats.visibilityRadius =
      config_->get()["simulation"]["rabbit"]["visibilityRadius"].get<u16>();

  for (u16 i = 0; i < config_->get()["simulation"]["rabbit"]["initialNumber"].get<u16>(); i++)
  {
    Coordinate position = getRandomWalkablePosition(terrainGenerator_.get(),
                                                 mapWidth_, mapHeight_);
    const auto id = generateId();
    const auto rabbit = std::make_shared<Rabbit>(id,
    position, resourceMap_.at("rabbit"), *terrainGenerator_.get(), stats, coordinateMap_);
    animals_.try_emplace(id,rabbit);
  }

  for (u16 i = 0;
       i < config_->get()["simulation"]["berries"]["initialNumber"].get<u16>();
       i++)
  {
    Coordinate position = getRandomPositionOfType(
        terrainGenerator_.get(), TerrainType::GRASS, mapWidth_, mapHeight_);
    const auto id = generateId();
    const auto plant = std::make_shared<Plant>(id, position, resourceMap_.at("berries"), coordinateMap_);
    plants_.try_emplace(id,plant);
  }
}

void Raygates::updateEntities()
{
  std::vector<u32> entitiesToDelete;
  entitiesToDelete.reserve(animals_.size() + plants_.size());

  for (const auto &[key, animal] : animals_)
  {
    if (!animal->update(expectedDeltaTime_))
    {
      entitiesToDelete.emplace_back(key);
    }
  }

  for (const auto &[key, plant] : plants_)
  {
    if (!plant->update())
    {
      entitiesToDelete.emplace_back(key);
    }
  }

  for (const auto& key : entitiesToDelete)
  {
    animals_.erase(key);
    plants_.erase(key);
  }
}

} // namespace raygates
