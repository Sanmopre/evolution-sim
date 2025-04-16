#include "wolf.h"


Wolf::Wolf(Vector2 position,std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats)
  : Animal(position, texture, terrainGenerator, stats)
{

}