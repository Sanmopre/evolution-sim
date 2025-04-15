#pragma once

#include "animal.h"

class Wolf : public Animal
{
public:
  Wolf(Vector2 position, const TerrainGenerator& terrainGenerator, const Stats& stats);

};



