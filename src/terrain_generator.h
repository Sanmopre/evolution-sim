#pragma once

// raylib
#include "raylib.h"

// std
#include <vector>
#include <map>


enum class TerrainType
{
  DEEP_WATER,
  WATER,
  GRASS,
  SAND,
  STONE,
  DIRT,
  SNOW
};

struct MinMaxTerrain {
  float min;
  float max;
};


class TerrainGenerator
{
public:
  TerrainGenerator(int width, int height);
  ~TerrainGenerator() = default;


  [[nodiscard]] const Texture2D& getTerrainTexture() const noexcept;

  [[nodiscard]] TerrainType getTerrainType(int x, int y) const noexcept;

private:

  Texture2D generatedTerrainTexture_;
  std::map<TerrainType, Color> colorMap_;
  std::map<TerrainType, MinMaxTerrain> rangeMap_;
  std::vector<std::vector<TerrainType>> terrainType_;
};
