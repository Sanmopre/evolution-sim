#include "rabbit.h"

Rabbit::Rabbit(u32 id, Coordinate position, std::shared_ptr<Texture2D> texture, const TerrainGenerator& terrainGenerator, const Stats& stats, std::unordered_map<Coordinate, std::vector<u32>>& coordinateMap)
  : Animal(id, position, texture, terrainGenerator, stats,coordinateMap)
{
  metrics_[HUNGER_KEY] = std::make_shared<Metric>(100.0f, 2.0f);
  metrics_[THIRST_KEY] = std::make_shared<Metric>(100.0f, 5.0f);
  metrics_[NEED_TO_REPRODUCE_KEY] = std::make_shared<Metric>(50.0f, 1.0f);
}