#include "rabbit.h"

Rabbit::Rabbit(Vector2 position, const TerrainGenerator& terrainGenerator, const Stats& stats)
  : Animal(position, "../resources/textures/rabbit.png", terrainGenerator, stats)
{

}