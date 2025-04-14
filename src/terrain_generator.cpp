
#include "terrain_generator.h"
#include "raylib.h"

// fastNoiseLite
#include "fastNoiseLite/fastNoiseLite.h"

// std
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
{

  colorMap_[TerrainType::DEEP_WATER] = DARKBLUE;
  colorMap_[TerrainType::WATER] = BLUE;
  colorMap_[TerrainType::GRASS] = GREEN;
  colorMap_[TerrainType::DIRT] = BROWN;
  colorMap_[TerrainType::SNOW] = RAYWHITE;
  colorMap_[TerrainType::SAND] = Color{239,239, 86, 255};
  colorMap_[TerrainType::STONE] = Color{152,152, 150, 255};

  rangeMap_[TerrainType::DEEP_WATER] = {0.0, 0.16};
  rangeMap_[TerrainType::WATER] = {0.16, 0.29};
  rangeMap_[TerrainType::SAND] = {0.29, 0.33};
  rangeMap_[TerrainType::DIRT] = {0.33, 0.37};
  rangeMap_[TerrainType::GRASS] = {0.37, 0.75};
  rangeMap_[TerrainType::STONE] = {0.75, 0.90};
  rangeMap_[TerrainType::SNOW] = {0.90, 1.0};

  // Create and configure FastNoise object
  FastNoiseLite noise;
  noise.SetSeed(34);
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  noise.SetFrequency(0.001);

  noise.SetFractalType(FastNoiseLite::FractalType_FBm);
  noise.SetFractalOctaves(8);
  noise.SetFractalLacunarity(1.9);

  // Create pixel data in RGBA format
  unsigned char *pixels = (unsigned char *)malloc(width * height * 4);

  for (int y = 0; y < height; y++)
  {
    std::vector<TerrainType> terrainTypeLine;
    terrainTypeLine.reserve(width);

    for (int x = 0; x < width; x++)
    {
      const auto noiseValue = (noise.GetNoise((float)x,(float)y) + 1.0)/2.0;
      Color tileColor = RAYWHITE;

      for (const auto& [key, value] : rangeMap_)
      {
        if (noiseValue >= value.min && noiseValue <= value.max)
        {
          tileColor = colorMap_[key];
          terrainTypeLine.emplace_back(key);
          break;
        }
      }

      const auto i = x + (y * width);
      pixels[(i * 4) + 0] = tileColor.r;
      pixels[(i * 4)+ 1] = tileColor.g + static_cast<int>(getRandomFloat());
      pixels[(i * 4) + 2] = tileColor.b + static_cast<int>(getRandomFloat());
      pixels[(i * 4) + 3] = tileColor.a + static_cast<int>(getRandomFloat());
    }
    terrainType_.emplace_back(terrainTypeLine);
  }


  // Create texture from image
  const Image image = {.data = pixels,
                 .width = width,
                 .height = height,
                 .mipmaps = 1,
                 .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

  generatedTerrainTexture_ = LoadTextureFromImage(image);
  UnloadImage(image);
}

const Texture2D &TerrainGenerator::getTerrainTexture() const noexcept
{
  return generatedTerrainTexture_;
}

TerrainType TerrainGenerator::getTerrainType(int x, int y) const noexcept
{
  return terrainType_[y][x];
}