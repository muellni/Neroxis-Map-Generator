#pragma once

#include <string>
#include "map/BinaryMask.h"

namespace faf {

class ConcurrentBinaryMask : public BinaryMask {
public:

  ConcurrentBinaryMask(int size, long seed, std::string const& name):
    BinaryMask(size, seed),
    name(name)
  {
  }

  ConcurrentBinaryMask(ConcurrentBinaryMask const& mask, long seed, std::string const& name):
    BinaryMask(mask, seed),
    name(name)
  {
  }

  std::string name;
};

}
