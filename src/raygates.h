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
  Config* config_;
  int windowWidth_;
  int windowHeight_;
  int mapWidth_;
  int mapHeight_;
  Window window;
  std::unique_ptr<TerrainGenerator> terrainGenerator_;
  std::vector<std::shared_ptr<Animal>> animals_;
  std::vector<std::shared_ptr<Plant>> plants_;

};

}
