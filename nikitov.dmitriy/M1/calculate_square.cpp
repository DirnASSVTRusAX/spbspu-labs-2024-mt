#include "calculate_square.hpp"
#include <algorithm>
#include <numeric>
#include <thread>
#include <future>
#include <random>
#include <vector>

size_t countPart(size_t radius, size_t seed, size_t tries, size_t id)
{
  std::default_random_engine randomizer(seed);
  std::uniform_real_distribution< double > distribution(0, radius);
  size_t count = 0;
  for (size_t i = 0; i != tries; ++i)
  {
    for (size_t j = 0; j != id * 2; ++j)
    {
      distribution(randomizer);
    }
    double x = distribution(randomizer);
    double y = distribution(randomizer);
    if (x * x + y * y <= radius * radius)
    {
      ++count;
    }
  }
  return count;
}

double nikitov::calculateSquare(size_t radius, size_t numberOfThreads, size_t seed, size_t tries)
{
  std::vector< std::future< size_t > > futures;
  futures.reserve(numberOfThreads - 1);
  std::vector< size_t > counts(numberOfThreads, 0);

  for (size_t i = 0; i != numberOfThreads - 1; ++i)
  {
    futures.emplace_back(std::async(countPart, radius, seed, tries / numberOfThreads, i));
  }
  counts.back() = countPart(radius, seed, tries / numberOfThreads + tries % numberOfThreads, numberOfThreads - 1);

  std::transform(futures.begin(), futures.end(), counts.begin(),
  [](auto && future)
  {
    return future.get();
  });

  return 4.0 * radius * radius * std::accumulate(counts.cbegin(), counts.cend(), 0) / tries;
}