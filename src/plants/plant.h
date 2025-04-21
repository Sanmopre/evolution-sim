#pragma once
#include "raylib.h"

// std
#include <memory>

#include "config.h"

class Plant
{
public:
  Plant(u32 id, Coordinate position, std::shared_ptr<Texture2D> texture,std::unordered_map<Coordinate, std::vector<u32>>& coordinateMap);
  ~Plant();

  [[nodiscard]] const Coordinate& getPosition() const noexcept;
  [[nodiscard]] const Texture2D& getTexture() const noexcept;

  [[nodiscard]] bool update();

private:
  const u32 id;
  std::unordered_map<Coordinate, std::vector<u32>>& coordinateMap_;
  Coordinate position_;
  std::shared_ptr<Texture2D> texture_;
};
