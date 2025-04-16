#pragma once

// raylib
#include "raylib.h"

// std
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

using Path = std::vector<Vector2>;

class TerrainGenerator
{
public:
  TerrainGenerator(int width, int height);
  void generate();
  void createTextureFromImage();
  ~TerrainGenerator() = default;

public:
  [[nodiscard]] float getLoadingProgress() const noexcept;

public:
  [[nodiscard]] const Texture2D& getTerrainTexture() const noexcept;
  [[nodiscard]] TerrainType getTerrainType(Vector2 point) const noexcept;
  [[nodiscard]] bool isWalkable(Vector2 point) const;

public:
  [[nodiscard]] std::optional<Path> getPathToDestination(Vector2 origin, Vector2 destination) const;
  [[nodiscard]] std::vector<Vector2> getTilesInRadius(Vector2 point, int radius) const;

private:
  [[nodiscard]] int heuristic(Vector2 pointA, Vector2 pointB) const noexcept;
  [[nodiscard]] Path reconstructPath(std::unordered_map<int, Vector2>& cameFrom,
    Vector2 end, int width) const;

private:
  int width_;
  int height_;
  Image image_;
  Texture2D generatedTerrainTexture_;
  std::map<TerrainType, Color> colorMap_;
  std::map<TerrainType, MinMaxTerrain> rangeMap_;
  std::vector<std::vector<TerrainType>> terrainType_;
  std::atomic<float> loadingProgress_;
};
