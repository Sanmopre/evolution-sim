#include "plant.h"

Plant::Plant(u32 id, Coordinate position, std::shared_ptr<Texture2D> texture,
             std::unordered_map<Coordinate, std::vector<u32>> &coordinateMap)
    : id(id), coordinateMap_(coordinateMap), position_(position), texture_(texture)
{
  coordinateMap_[position_].emplace_back(id);
}

Plant::~Plant()
{
  if (coordinateMap_.find(position_) != coordinateMap_.end())
  {
    auto& vector = coordinateMap_.at(position_);
    vector.erase(std::remove(vector.begin(), vector.end(), id), vector.end());
    if (vector.empty())
    {
      coordinateMap_.erase(position_);
    }
  }
}

const Coordinate &Plant::getPosition() const noexcept
{
  return position_;
}

const Texture2D &Plant::getTexture() const noexcept
{
  return *texture_;
}

bool Plant::update()
{
  return true;
}