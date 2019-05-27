#include <chrono>
#include <iostream>

#include "map/BinaryMask.h"


int main(int argc, char *argv[])
{
  static constexpr std::size_t size = 4096;
  auto start = std::chrono::system_clock::now();
  auto mark = faf::BinaryMask(size, 123)
              .randomize(0.4f)
              .acid(0.4f)
              .deflate(0.2f)
              .inflate(0.2f)
              .cutCorners()
              .enlarge(2*size)
              .invert()
              .shrink(size)
              .outline()
              .smooth(2.f);
  auto end = std::chrono::system_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << elapsed.count() << std::endl;
  return 0;
}
