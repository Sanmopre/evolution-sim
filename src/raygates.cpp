//
// Created by sanmopre on 4/10/25.
//

#include "raygates.h"

#include "animals/rabbit.h"
#include "animals/wolf.h"

#include <global.h>
#include <random>

namespace raygates
{


namespace {

float getRandomFloat(float min = -1.0f, float max = 1.0f)
{
  static std::random_device rd;
  static std::mt19937 gen(rd()); // Mersenne Twister engine
  std::uniform_real_distribution<float> dist(min, max);
  return dist(gen);
}

}

Raygates::Raygates()
  : window(1920, 1080, PROJECT_NAME)
{

  terrainGenerator_ = std::make_unique<TerrainGenerator>(1920, 1080);

  for (int i = 0; i < 1000; i++)
  {
    Vector2 position{getRandomFloat(0.0f, 1920.0f), getRandomFloat(0.0f, 1080.0f)};
    animals_.emplace_back(std::make_shared<Rabbit>(position));
  }
}

Raygates::~Raygates()
{

}

bool Raygates::update()
{
  for (const auto& animal : animals_)
  {
    Vector2 position {getRandomFloat(-1.0f, 1.0f), getRandomFloat(-1.0f, 1.0f)};
    animal->setNextRelativePosition(position);
  }

  return window.render(animals_, terrainGenerator_.get());
}

} // namespace raygates
