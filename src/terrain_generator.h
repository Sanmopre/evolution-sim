#pragma once

// raylib
#include "raylib.h"

// std
#include "config.h"

#include <atomic>
#include <cmath>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <unordered_map>
#include <vector>

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

struct MinMaxTerrain
{
  f32 min;
  f32 max;
};


struct Node
{
  Coordinate point;
  i64 g, f;
  bool operator>(const Node& other) const
  {
    return f > other.f;
  }
};

using Path = std::vector<Coordinate>;

class TerrainGenerator
{
public:
  TerrainGenerator(u16 width, u16 height);
  void generate();
  void createTextureFromImage();
  ~TerrainGenerator() = default;

public:
  [[nodiscard]] float getLoadingProgress() const noexcept;

public:
  [[nodiscard]] const Texture2D& getTerrainTexture() const noexcept;
  [[nodiscard]] TerrainType getTerrainType(Coordinate point) const noexcept;
  [[nodiscard]] bool isWalkable(Coordinate point) const;

public:
  [[nodiscard]] std::optional<Path> getPathToDestination(Coordinate origin, Coordinate destination) const;
  [[nodiscard]] std::vector<Coordinate> getTilesInRadius(Coordinate point, u16 radius) const;

private:
  [[nodiscard]] i64 heuristic(Coordinate pointA, Coordinate pointB) const noexcept;
  [[nodiscard]] Path reconstructPath(std::unordered_map<i64, Coordinate>& cameFrom,
    Coordinate end, u16 width) const;

private:
  u16 width_;
  u16 height_;
  Image image_;
  Texture2D generatedTerrainTexture_;
  std::map<TerrainType, Color> colorMap_;
  std::map<TerrainType, MinMaxTerrain> rangeMap_;
  std::vector<std::vector<TerrainType>> terrainType_;
  std::atomic<f32> loadingProgress_;
};
