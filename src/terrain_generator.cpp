
#include "terrain_generator.h"
#include "raylib.h"

// fastNoiseLite
#include "fastNoiseLite/fastNoiseLite.h"

TerrainGenerator::TerrainGenerator(int width, int height)
{

  colorMap_[TerrainType::DEEP_WATER] = DARKBLUE;
  colorMap_[TerrainType::WATER] = BLUE;
  colorMap_[TerrainType::GRASS] = GREEN;
  colorMap_[TerrainType::DIRT] = BROWN;
  colorMap_[TerrainType::SNOW] = RAYWHITE;
  colorMap_[TerrainType::SAND] = YELLOW;
  colorMap_[TerrainType::STONE] = YELLOW;

  rangeMap_[TerrainType::DEEP_WATER] = {0.0, 0.16};
  rangeMap_[TerrainType::WATER] = {0.16, 0.2};
  rangeMap_[TerrainType::SAND] = {0.2, 0.23};
  rangeMap_[TerrainType::DIRT] = {0.23, 0.27};
  rangeMap_[TerrainType::GRASS] = {0.27, 0.8};
  rangeMap_[TerrainType::STONE] = {0.8, 0.95};
  rangeMap_[TerrainType::SNOW] = {0.95, 1.0};

  // Create and configure FastNoise object
  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  noise.SetFrequency(0.001);

  noise.SetFractalType(FastNoiseLite::FractalType_FBm);
  noise.SetFractalOctaves(8);
  noise.SetFractalLacunarity(1.9);

  // Step 1: Create pixel data in RGBA format
  unsigned char *pixels =
      (unsigned char *)malloc(width * height * 4); // 4 bytes per pixel

  float x = -1.0;
  float y = -1.0;

  for (int i = 0; i < width * height; i++)
  {
    if (i % (width) == 0)
    {
      x = -1.0;
      y += 1;
    }

    x += 1.0;

    const auto noiseValue = (noise.GetNoise(x,y) + 1.0)/2.0;
    Color tileColor = RAYWHITE;

    for (const auto& [key, value] : rangeMap_)
    {
      if (noiseValue >= value.min && noiseValue <= value.max)
      {
        tileColor = colorMap_[key];
        break;
      }
    }

    pixels[i * 4 + 0] = tileColor.r;
    pixels[i * 4 + 1] = tileColor.g;
    pixels[i * 4 + 2] = tileColor.b;
    pixels[i * 4 + 3] = tileColor.a;
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