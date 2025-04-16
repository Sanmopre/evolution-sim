
#include "terrain_generator.h"
#include "raylib.h"

// fastNoiseLite
#include "fastNoiseLite/fastNoiseLite.h"

// std
#include <algorithm>
#include <iostream>
#include <ostream>
#include <random>

namespace {

float getRandomFloat(float min = -6.0f, float max = 0.0f) {
  static std::random_device rd;
  static std::mt19937 gen(rd()); // Mersenne Twister engine
  std::uniform_real_distribution<float> dist(min, max);
  return dist(gen);
}

}

TerrainGenerator::TerrainGenerator(int width, int height)
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
  unsigned char *pixels = (unsigned char *)malloc(width_ * height_ * 4);

  for (int y = 0; y < height_; y++)
  {
    std::vector<TerrainType> terrainTypeLine;
    terrainTypeLine.reserve(width_);

    for (int x = 0; x < width_; x++)
    {
      const auto noiseValue = (noise.GetNoise((float)x, (float)y) + 1.0) / 2.0;
      Color tileColor = RAYWHITE;

      for (const auto &[key, value] : rangeMap_) {
        if (noiseValue >= value.min && noiseValue <= value.max) {
          tileColor = colorMap_[key];
          terrainTypeLine.emplace_back(key);
          break;
        }
      }

      const auto i = x + (y * width_);
      pixels[(i * 4) + 0] = tileColor.r;
      pixels[(i * 4) + 1] = tileColor.g + static_cast<int>(getRandomFloat());
      pixels[(i * 4) + 2] = tileColor.b + static_cast<int>(getRandomFloat());
      pixels[(i * 4) + 3] = tileColor.a + static_cast<int>(getRandomFloat());

      loadingProgress_ = static_cast<float>(i) / static_cast<float>(width_ * height_) * 100.0;
    }
    terrainType_.emplace_back(terrainTypeLine);
  }
  loadingProgress_ = 100.0f;

   image_ = {.data = pixels,
                       .width = width_,
                       .height = height_,
                       .mipmaps = 1,
                       .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
}

void TerrainGenerator::createTextureFromImage() {
  generatedTerrainTexture_ = LoadTextureFromImage(image_);
  UnloadImage(image_);
}

float TerrainGenerator::getLoadingProgress() const noexcept
{
  return loadingProgress_;
}

const Texture2D &TerrainGenerator::getTerrainTexture() const noexcept
{
  return generatedTerrainTexture_;
}

TerrainType TerrainGenerator::getTerrainType(Vector2 point) const noexcept
{
  return terrainType_[static_cast<int>(point.y)][static_cast<int>(point.x)];
}

bool TerrainGenerator::isWalkable(Vector2 point) const {
  if (point.y < 0 || point.y >= terrainType_.size() || point.x < 0 ||
      point.x >= terrainType_[0].size())
    return false;

  const TerrainType t = getTerrainType(point);
  return !(t == TerrainType::WATER || t == TerrainType::DEEP_WATER);
}

int TerrainGenerator::heuristic(Vector2 pointA, Vector2 pointB) const noexcept {
  return std::abs(pointA.x - pointB.x) + std::abs(pointA.y - pointB.y);
}



Path TerrainGenerator::reconstructPath(std::unordered_map<int, Vector2> &cameFrom,
                                  Vector2 end, int width) const
{
  Path path;
  int currentKey = end.y * width + end.x;

  while (cameFrom.find(currentKey) != cameFrom.end()) {
    auto point = cameFrom[currentKey];
    path.emplace_back(point);
    currentKey = point.y * width + point.x;
  }

  std::reverse(path.begin(), path.end());
  return path;
}


std::optional<Path>
TerrainGenerator::getPathToDestination(Vector2 origin,
                                       Vector2 destination) const {
  if (!isWalkable(destination))
  {
    return std::nullopt;
  }

  int width = terrainType_[0].size();

  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
  std::unordered_map<int, int> gScore;
  std::unordered_map<int, Vector2> cameFrom;

  auto key = [&](int x, int y) { return y * width + x; };

  openSet.push({origin.x, origin.y, 0, heuristic(origin, destination)});
  gScore[key(origin.x, origin.y)] = 0;

  const int dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // 4-way movement

  while (!openSet.empty()) {
    Node current = openSet.top();
    openSet.pop();

    if (current.point.x == destination.x && current.point.y == destination.y)
      return reconstructPath(cameFrom, destination, width);

    for (auto &dir : dirs) {
      Vector2 n = current.point;
      n.x += dir[0];
      n.y += dir[1];

      if (!isWalkable(n))
        continue;

      int tentative_g = gScore[key(current.point.x, current.point.y)] + 1;

      if (gScore.find(key(n.x, n.y)) == gScore.end() ||
          tentative_g < gScore[key(n.x, n.y)]) {
        cameFrom[key(n.x, n.y)] = {current.point.x, current.point.y};
        gScore[key(n.x, n.y)] = tentative_g;
        int f = tentative_g + heuristic(n, destination);
        openSet.push({n.x, n.y, tentative_g, f});
      }
    }
  }

  return {}; // no path found
}

std::vector<Vector2> TerrainGenerator::getTilesInRadius(Vector2 point,
                                                        int radius) const
{
  std::vector<Vector2> result;

  int minY = std::max(0, static_cast<int>(point.y - radius));
  int maxY = std::min((int)terrainType_.size() - 1, static_cast<int>(point.y + radius));
  int minX = std::max(0, static_cast<int>(point.x - radius));
  int maxX = std::min((int)terrainType_[0].size() - 1, static_cast<int>(point.x + radius));

  float radiusSq = radius * radius;

  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      float dx = point.x - x;
      float dy = point.y - y;
      if (dx * dx + dy * dy <= radiusSq) {
        result.emplace_back(Vector2{x, y});
      }
    }
  }

  return result;
}