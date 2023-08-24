#include <array>
#include <charconv>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>

auto to_int_except(const std::string& s) noexcept(false) {
  return std::stoi(s);
}

auto to_int_noexcept(const std::string& s) noexcept {
  int result;
  auto [_, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
  return make_pair(result, ec);
}

int main() {
  constexpr char alpha[] = "abcdefghijklmnopqrstuvwxyz";
  constexpr char num[] = "0123456789";
  constexpr int num_runs = 10000;
  constexpr int digits = 5;
  std::srand(std::time(nullptr));
  std::array<std::string, num_runs> good_nums, bad_nums;
  for (size_t i = 0; i < num_runs; ++i) {
    for (size_t j = 0; j < digits; ++j) {
      good_nums[i].push_back(num[std::rand() % (sizeof(num) - 1)]);
      bad_nums[i].push_back(alpha[std::rand() % (sizeof(alpha) - 1)]);
    }
  }

  auto t1 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < num_runs; ++i) {
    auto _ = to_int_noexcept(good_nums[i]);
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  std::cout
      << "good_nums(no exception): "
      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
      << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < num_runs; ++i) {
    auto _ = to_int_noexcept(bad_nums[i]);
  }
  t2 = std::chrono::high_resolution_clock::now();
  std::cout
      << "bad_nums(no exception): "
      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
      << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < num_runs; ++i) {
    try {
      auto _ = to_int_except(good_nums[i]);
    } catch (const std::invalid_argument& e) {
      // ignore
    }
  }
  t2 = std::chrono::high_resolution_clock::now();
  std::cout
      << "good_nums(exception): "
      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
      << std::endl;

  t1 = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < num_runs; ++i) {
    try {
      auto _ = to_int_except(bad_nums[i]);
    } catch (const std::invalid_argument& e) {
      // ignore
    }
  }
  t2 = std::chrono::high_resolution_clock::now();
  std::cout
      << "bad_nums(exception): "
      << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
      << std::endl;

  return 0;
}
