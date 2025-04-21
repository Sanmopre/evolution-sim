#include "rabbit.h"

Rabbit::Rabbit(Coordinate position, std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats, std::unordered_map<Coordinate, std::vector<u32>>& coordinateMap)
  : Animal(position, texture, terrainGenerator, stats,coordinateMap)
{

}