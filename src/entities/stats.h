#pragma once

#include "entity_manager.h"

[[nodiscard]] std::pair<Metrics, Stats> getRabbitData()
{
  Metrics metrics;
  metrics[MetricKey::HUNGER_KEY] = std::make_shared<Metric>(100.0f, 2.0f);
  metrics[MetricKey::THIRST_KEY] = std::make_shared<Metric>(100.0f, 5.0f);
  metrics[MetricKey::NEED_TO_REPRODUCE_KEY] = std::make_shared<Metric>(50.0f, 1.0f);

  Stats stats;
  stats[StatsKey::SPEED] = 5.0f;
  stats[StatsKey::VISIBILITY_RANGE] = 15.0f;

  return {metrics, stats};
}

[[nodiscard]] std::pair<Metrics, Stats> getWolfData()
{
  Metrics metrics;
  metrics[MetricKey::HUNGER_KEY] = std::make_shared<Metric>(50.0f, 2.0f);
  metrics[MetricKey::THIRST_KEY] = std::make_shared<Metric>(70.0f, 5.0f);
  metrics[MetricKey::NEED_TO_REPRODUCE_KEY] = std::make_shared<Metric>(50.0f, 1.0f);

  Stats stats;
  stats[StatsKey::SPEED] = 15.0f;
  stats[StatsKey::VISIBILITY_RANGE] = 30.0f;

  return {metrics, stats};
}

[[nodiscard]] std::pair<Metrics, Stats> getDeerData()
{
  Metrics metrics;
  metrics[MetricKey::HUNGER_KEY] = std::make_shared<Metric>(120.0f, 2.0f);
  metrics[MetricKey::THIRST_KEY] = std::make_shared<Metric>(120.0f, 5.0f);
  metrics[MetricKey::NEED_TO_REPRODUCE_KEY] = std::make_shared<Metric>(50.0f, 1.0f);

  Stats stats;
  stats[StatsKey::SPEED] = 17.0f;
  stats[StatsKey::VISIBILITY_RANGE] = 10.0f;

  return {metrics, stats};
}

[[nodiscard]] std::pair<Metrics, Stats> getBearData()
{
  Metrics metrics;
  metrics[MetricKey::HUNGER_KEY] = std::make_shared<Metric>(400.0f, 2.0f);
  metrics[MetricKey::THIRST_KEY] = std::make_shared<Metric>(230.0f, 5.0f);
  metrics[MetricKey::NEED_TO_REPRODUCE_KEY] = std::make_shared<Metric>(100.0f, 1.0f);

  Stats stats;
  stats[StatsKey::SPEED] = 3.0f;
  stats[StatsKey::VISIBILITY_RANGE] = 30.0f;

  return {metrics, stats};
}



