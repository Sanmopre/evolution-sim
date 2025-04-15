#include "wolf.h"


Wolf::Wolf(Vector2 position, const TerrainGenerator& terrainGenerator, const Stats& stats)
  : Animal(position, "../resources/textures/wolf.png", terrainGenerator, stats)
{

}