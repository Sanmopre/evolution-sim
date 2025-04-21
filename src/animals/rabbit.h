#pragma once

#include "animal.h"

class Rabbit : public Animal
{
public:
  Rabbit(u32 id, Coordinate position, std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats, std::unordered_map<Coordinate, std::vector<u32>>& coordinateMap);

};



