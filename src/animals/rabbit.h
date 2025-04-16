#pragma once

#include "animal.h"

class Rabbit : public Animal
{
public:
  Rabbit(Vector2 position, std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats);

};



