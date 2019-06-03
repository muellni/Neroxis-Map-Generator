#pragma once

#include <string>
#include "map/FloatMask.h"

namespace faf {

class ConcurrentFloatMask : public FloatMask {
public:
  ConcurrentFloatMask(int size, long seed, std::string name):
    FloatMask(size, seed),
    name(name)
  {
  }

  std::string name;
};

}
