#include <chrono>
#include <thread>
#include "Arduino.h"

auto start_time = std::chrono::high_resolution_clock::now();

unsigned int millis(void)
{
  auto current_time = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
}

unsigned int micros(void)
{
  auto current_time = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::microseconds>(current_time - start_time).count();
}
