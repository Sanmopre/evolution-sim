#pragma once

// raylib
#include "raylib.h"

// std
#include <map>
#include <optional>
#include <vector>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <limits>

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


struct Node
{
  Vector2 point;
  int g, f;
  bool operator>(const Node& other) const
  {
    return f > other.f;
  }
};


class TerrainGenerator
{
public:
  TerrainGenerator(int width, int height);
  ~TerrainGenerator() = default;

public:
  [[nodiscard]] const Texture2D& getTerrainTexture() const noexcept;
  [[nodiscard]] TerrainType getTerrainType(Vector2 point) const noexcept;

public:
  [[nodiscard]] std::optional<std::vector<Vector2>> getPathToDestination(Vector2 origin, Vector2 destination) const;

private:
  [[nodiscard]] bool isWalkable(Vector2 point) const;
  [[nodiscard]] int heuristic(Vector2 pointA, Vector2 pointB) const noexcept;
  [[nodiscard]] std::vector<Vector2> reconstructPath(std::unordered_map<int, Vector2>& cameFrom,
    Vector2 end, int width) const;

private:

  Texture2D generatedTerrainTexture_;
  std::map<TerrainType, Color> colorMap_;
  std::map<TerrainType, MinMaxTerrain> rangeMap_;
  std::vector<std::vector<TerrainType>> terrainType_;
};
