//
// Created by sanmopre on 4/10/25.
//

#include "evolution-sim.h"

#include <global.h>
#include <iostream>
#include <ostream>
#include <random>

#include "entities/stats.h"

namespace evo_sim {

std::unordered_map<Coordinate, std::vector<u32>> coordinateMap_;

namespace {

[[nodiscard]] u32 generateId() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<u32> dist;
  return dist(gen);
}

[[nodiscard]] Coordinate getRandomWalkablePosition(TerrainGenerator *terrain,
                                                   u16 width, u16 height) {
  if (const Coordinate position{GetRandomValue(0, width),
                                GetRandomValue(0, height)};
      terrain->isWalkable(position)) {
    return position;
  }

  return getRandomWalkablePosition(terrain, width, height);
}

[[nodiscard]] Coordinate getRandomPositionOfType(TerrainGenerator *terrain,
                                                 TerrainType type, u16 width,
                                                 u16 height) {
  if (const Coordinate position{GetRandomValue(0, width - 1),
                                GetRandomValue(0, height - 1)};
      terrain->getTerrainType(position) == type) {
    return position;
  }

  return getRandomPositionOfType(terrain, type, width, height);
}

} // namespace

EvolutionSim::EvolutionSim(Config *config)
    : config_(config),
      windowWidth_(config->get()["window"]["width"].get<u16>()),
      windowHeight_(config->get()["window"]["height"].get<u16>()),
      mapWidth_(config->get()["map"]["width"].get<u16>()),
      mapHeight_(config->get()["map"]["height"].get<u16>()),
      targetFPS_(config->get()["targetFPS"].get<u16>()),
      expectedDeltaTime_(1.0f / static_cast<f32>(targetFPS_)),
      window(windowWidth_, windowHeight_, PROJECT_NAME, targetFPS_)
{
  terrainGenerator_ = std::make_unique<TerrainGenerator>(mapWidth_, mapHeight_);
  entityManager_ = std::make_unique<EntityManager>(*terrainGenerator_);
  terrainLoadingThread_ =
      std::thread(&TerrainGenerator::generate, terrainGenerator_.get());
}

EvolutionSim::~EvolutionSim()
{
  terrainLoadingThread_.join();
}

bool EvolutionSim::update()
{
  if (!hasTerrainFinishedGenerating_) {
    if (terrainGenerator_->getLoadingProgress() >= 100.0f) {
      terrainLoadingThread_.join();
      terrainGenerator_->createTextureFromImage();
      initSimulation();
      hasTerrainFinishedGenerating_ = true;
      return true;
    }

    hasTerrainFinishedGenerating_ = false;
    return window.renderLoadingScreen(terrainGenerator_->getLoadingProgress());
  }

  if (!uiState_.simulationStop) {
    for (u8 i = 0; i < static_cast<u8>(uiState_.simulationSpeedSlider); i++)
    {
      entityManager_->updateEntities(expectedDeltaTime_);
    }
  }

  uiState_ = window.render(entityManager_->getRegistry(), terrainGenerator_.get());

  return !uiState_.windowShouldClose;
}

void EvolutionSim::initSimulation()
{
  const auto rabbitData = getRabbitData();

  for (u16 i = 0;
       i < config_->get()["simulation"]["rabbits"].get<u16>();
       i++)
  {
    Coordinate position = getRandomWalkablePosition(terrainGenerator_.get(),
                                                mapWidth_, mapHeight_);

    entityManager_->createEntity(AnimalType::RABBIT, position, rabbitData.first, rabbitData.second);
  }

  const auto wolfData = getWolfData();

  for (u16 i = 0;
       i < config_->get()["simulation"]["wolves"].get<u16>();
       i++)
  {
    Coordinate position = getRandomWalkablePosition(terrainGenerator_.get(),
                                                mapWidth_, mapHeight_);

    entityManager_->createEntity(AnimalType::WOLF, position, wolfData.first, wolfData.second);
  }

  const auto deerData = getDeerData();

  for (u16 i = 0;
       i < config_->get()["simulation"]["deers"].get<u16>();
       i++)
  {
    Coordinate position = getRandomWalkablePosition(terrainGenerator_.get(),
                                                mapWidth_, mapHeight_);

    entityManager_->createEntity(AnimalType::DEER, position, deerData.first, deerData.second);
  }

  const auto bearData = getBearData();

  for (u16 i = 0;
       i < config_->get()["simulation"]["bears"].get<u16>();
       i++)
  {
    Coordinate position = getRandomWalkablePosition(terrainGenerator_.get(),
                                                mapWidth_, mapHeight_);

    entityManager_->createEntity(AnimalType::BEAR, position, bearData.first, bearData.second);
  }

}


} // namespace raygates
