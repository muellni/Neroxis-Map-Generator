#pragma once

#include <ostream>

#include "generator/Random.h"
#include "test/config.h"

namespace faf {

static auto gen_rand_int = [](std::ostream& out)
{
  Random random(seed);
  for (int i = 0; i < 1000; ++i)
  {
    out << random.nextInt() << "\n";
  }
};

static auto gen_rand_int_bound = [](std::ostream& out)
{
  Random random(seed);
  for (int i = 1; i <= 100; ++i)
  {
    out << random.nextInt(i) << "\n";
  }
};

static auto gen_rand_long = [](std::ostream& out)
{
  Random random(seed);
  for (int i = 0; i < 1000; ++i)
  {
    out << random.nextLong() << "\n";
  }
};

static auto gen_rand_float = [](std::ostream& out)
{
  out.precision(3);
  out.setf( std::ios::fixed, std:: ios::floatfield );
  Random random(seed);
  for (int i = 0; i < 1000; ++i)
  {
    out << random.nextFloat() << "\n";
  }
};

}
