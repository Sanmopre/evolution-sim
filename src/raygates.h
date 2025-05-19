#pragma once

#include "config.h"

#include "terrain_generator.h"
#include "window.h"

#include "entities/entity_manager.h"

namespace raygates
{

class Raygates
{
public:
  Raygates(Config* config);
  ~Raygates();

  [[nodiscard]] bool update();

private:
  void initSimulation();
  void updateEntities();

private:
  std::thread terrainLoadingThread_;
  Config* config_;
  bool hasTerrainFinishedGenerating_ = false;

private:
  // Window
  u16 windowWidth_;
  u16 windowHeight_;
  u16 mapWidth_;
  u16 mapHeight_;
  u16 targetFPS_ = 0;
  f32 expectedDeltaTime_;
  Window window;
  UIState uiState_;

private:
  // Entity maps
  std::unique_ptr<TerrainGenerator> terrainGenerator_;

private:
  std::unordered_map<Coordinate, std::vector<u32>> coordinateMap_;
  std::unique_ptr<EntityManager> entityManager_;
};

}
