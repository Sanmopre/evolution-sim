#pragma once

#include "animals/animal.h"
#include "terrain_generator.h"
#include "window.h"

namespace raygates
{

class Raygates
{
public:
  Raygates();
  ~Raygates();

  [[nodiscard]] bool update();

private:
  Window window;
  std::unique_ptr<TerrainGenerator> terrainGenerator_;
  std::vector<std::shared_ptr<Animal>> animals_;
};

}
