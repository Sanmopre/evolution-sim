#pragma once

#include "animal.h"

class Rabbit : public Animal
{
public:
  Rabbit(Vector2 position, const TerrainGenerator& terrainGenerator, const Stats& stats);

};



