#pragma once

#include "animal.h"

class Rabbit : public Animal
{
public:
  Rabbit(Coordinate position, std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats);

};



