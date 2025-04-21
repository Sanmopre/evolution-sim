#pragma once

#include "animal.h"

class Wolf : public Animal
{
public:
  Wolf(Coordinate position, std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats);

};



