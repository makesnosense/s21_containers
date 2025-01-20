#ifndef RANDOM_MT_H
#define RANDOM_MT_H

#include <chrono>
#include <random>

namespace Random {

inline std::mt19937 GenerateSeedSequence() {
  std::random_device system_entropy{};
  const auto time_seed = static_cast<std::seed_seq::result_type>(
      std::chrono::steady_clock::now().time_since_epoch().count());

  std::seed_seq seed_sequence{
      time_seed,        system_entropy(), system_entropy(), system_entropy(),
      system_entropy(), system_entropy(), system_entropy(), system_entropy()};

  return std::mt19937{seed_sequence};
}

inline std::mt19937 mt{GenerateSeedSequence()};

inline int Get(const int min, const int max) {
  return std::uniform_int_distribution{min, max}(mt);
}

}  // namespace Random

#endif