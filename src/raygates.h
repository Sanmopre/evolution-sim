#pragma once

#include "animals/animal.h"
#include "config.h"
#include "plants/plant.h"

#include "terrain_generator.h"
#include "window.h"

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

private:
  std::thread terrainLoadingThread_;
  Config* config_;
  bool hasTerrainFinishedGenerating_ = false;

private:
  // Window
  int windowWidth_;
  int windowHeight_;
  int mapWidth_;
  int mapHeight_;
  int targetFPS_ = 0;
  Window window;

private:
  // Entity maps
  std::unique_ptr<TerrainGenerator> terrainGenerator_;
  std::vector<std::shared_ptr<Animal>> animals_;
  std::vector<std::shared_ptr<Plant>> plants_;

private:
  std::unordered_map<std::string, std::shared_ptr<Texture2D>> resourceMap_;
};

}
