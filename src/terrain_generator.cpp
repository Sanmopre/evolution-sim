
#include "terrain_generator.h"
#include "raylib.h"

// fastNoiseLite
#include "fastNoiseLite.h"

// std
#include <algorithm>
#include <iostream>
#include <ostream>
#include <random>

namespace {

f32 getRandomf32(f32 min = -20.0f, f32 max = 0.0f) {
  static std::random_device rd;
  static std::mt19937 gen(rd()); // Mersenne Twister engine
  std::uniform_real_distribution<f32> dist(min, max);
  return dist(gen);
}

}

TerrainGenerator::TerrainGenerator(u16 width, u16 height)
  : width_(width), height_(height)
{

}

void TerrainGenerator::generate() {

  colorMap_[TerrainType::DEEP_WATER] = DARKBLUE;
  colorMap_[TerrainType::WATER] = BLUE;
  colorMap_[TerrainType::GRASS] = GREEN;
  colorMap_[TerrainType::DIRT] = BROWN;
  colorMap_[TerrainType::SNOW] = RAYWHITE;
  colorMap_[TerrainType::SAND] = Color{239, 239, 86, 255};
  colorMap_[TerrainType::STONE] = Color{152, 152, 150, 255};

  rangeMap_[TerrainType::DEEP_WATER] = {0.0, 0.16};
  rangeMap_[TerrainType::WATER] = {0.16, 0.29};
  rangeMap_[TerrainType::SAND] = {0.29, 0.33};
  rangeMap_[TerrainType::DIRT] = {0.33, 0.37};
  rangeMap_[TerrainType::GRASS] = {0.37, 0.75};
  rangeMap_[TerrainType::STONE] = {0.75, 0.90};
  rangeMap_[TerrainType::SNOW] = {0.90, 1.0};

  // Create and configure FastNoise object
  FastNoiseLite noise;
  noise.SetSeed(35);
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  noise.SetFrequency(0.001);

  noise.SetFractalType(FastNoiseLite::FractalType_FBm);
  noise.SetFractalOctaves(8);
  noise.SetFractalLacunarity(1.9);

  // Create pixel data in RGBA format
  const auto pixels =  static_cast<unsigned char *>(malloc(width_ * height_ * 4));

  for (i64 y = 0; y < height_; y++)
  {
    std::vector<TerrainType> terrainTypeLine;
    terrainTypeLine.reserve(width_);

    for (i64 x = 0; x < width_; x++)
    {
      const auto noiseValue = (noise.GetNoise(static_cast<f32>(x), static_cast<f32>(y)) + 1.0) / 2.0;
      auto tileColor = RAYWHITE;

      for (const auto &[key, value] : rangeMap_) {
        if (noiseValue >= value.min && noiseValue <= value.max) {
          tileColor = colorMap_[key];
          terrainTypeLine.emplace_back(key);
          break;
        }
      }

      const auto tile = x + (y * width_);
      pixels[(tile * 4) + 0] = tileColor.r;
      pixels[(tile * 4) + 1] = tileColor.g + static_cast<i64>(getRandomf32());
      pixels[(tile * 4) + 2] = tileColor.b + static_cast<i64>(getRandomf32());
      pixels[(tile * 4) + 3] = tileColor.a + static_cast<i64>(getRandomf32());

      loadingProgress_ = static_cast<f32>(tile) / static_cast<f32>(width_ * height_) * 100.0;
    }
    terrainType_.emplace_back(terrainTypeLine);
  }
  loadingProgress_ = 100.0f;

  image_ = {
    pixels,
    width_,
    height_,
    1,
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
};
}

void TerrainGenerator::createTextureFromImage()
{
  generatedTerrainTexture_ = LoadTextureFromImage(image_);
  UnloadImage(image_);
}

f32 TerrainGenerator::getLoadingProgress() const noexcept
{
  return loadingProgress_;
}

const Texture2D &TerrainGenerator::getTerrainTexture() const noexcept
{
  return generatedTerrainTexture_;
}

TerrainType TerrainGenerator::getTerrainType(Coordinate point) const noexcept
{
  return terrainType_[point.y][point.x];
}

bool TerrainGenerator::isWalkable(Coordinate point) const {
  if (point.y < 0 || point.y >= terrainType_.size() || point.x < 0 ||
      point.x >= terrainType_[0].size())
    return false;

  const TerrainType t = getTerrainType(point);
  return !(t == TerrainType::WATER || t == TerrainType::DEEP_WATER);
}

i64 TerrainGenerator::heuristic(Coordinate pointA, Coordinate pointB) const noexcept {
  return std::abs(pointA.x - pointB.x) + std::abs(pointA.y - pointB.y);
}



Path TerrainGenerator::reconstructPath(std::unordered_map<i64, Coordinate> &cameFrom,
                                  Coordinate end, u16 width) const
{
  Path path;
  i64 currentKey = end.y * width + end.x;

  while (cameFrom.find(currentKey) != cameFrom.end()) {
    auto point = cameFrom[currentKey];
    path.emplace_back(point);
    currentKey = point.y * width + point.x;
  }

  std::reverse(path.begin(), path.end());
  return path;
}


std::optional<Path>
TerrainGenerator::getPathToDestination(Coordinate origin,
                                       Coordinate destination) const {
  if (!isWalkable(destination))
  {
    return std::nullopt;
  }

  i64 width = terrainType_[0].size();

  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
  std::unordered_map<i64, i64> gScore;
  std::unordered_map<i64, Coordinate> cameFrom;

  auto key = [&](i64 x, i64 y) { return y * width + x; };

  openSet.push({origin.x, origin.y, 0, heuristic(origin, destination)});
  gScore[key(origin.x, origin.y)] = 0;

  const i64 dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // 4-way movement

  while (!openSet.empty()) {
    Node current = openSet.top();
    openSet.pop();

    if (current.point.x == destination.x && current.point.y == destination.y)
      return reconstructPath(cameFrom, destination, width);

    for (auto &dir : dirs) {
      Coordinate n = current.point;
      n.x += dir[0];
      n.y += dir[1];

      if (!isWalkable(n))
        continue;

      i64 tentative_g = gScore[key(current.point.x, current.point.y)] + 1;

      if (gScore.find(key(n.x, n.y)) == gScore.end() ||
          tentative_g < gScore[key(n.x, n.y)]) {
        cameFrom[key(n.x, n.y)] = {current.point.x, current.point.y};
        gScore[key(n.x, n.y)] = tentative_g;
        i64 f = tentative_g + heuristic(n, destination);
        openSet.push({n.x, n.y, tentative_g, f});
      }
    }
  }

  return {}; // no path found
}

std::vector<Coordinate> TerrainGenerator::getTilesInRadius(Coordinate point,
                                                        u16 radius) const
{
  std::vector<Coordinate> result;

  i64 minY = std::max(0, static_cast<i32>(point.y - radius));
  i64 maxY = std::min(static_cast<i64>(terrainType_.size() - 1), (point.y + radius));
  i64 minX = std::max(0, static_cast<i32>(point.x - radius));
  i64 maxX = std::min(static_cast<i64>(terrainType_[0].size() - 1), (point.x + radius));

  f32 radiusSq = radius * radius;

  for (i64 y = minY; y <= maxY; ++y) {
    for (i64 x = minX; x <= maxX; ++x) {
      f32 dx = point.x - x;
      f32 dy = point.y - y;
      if (dx * dx + dy * dy <= radiusSq) {
        result.emplace_back(Coordinate{x, y});
      }
    }
  }

  return result;
}