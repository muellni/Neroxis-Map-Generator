#pragma once

#include "map/BinaryMask.h"
#include "map/FloatMask.h"
#include "test/config.h"
#include "test/gen_BinaryMask.h"

namespace faf {

static constexpr std::size_t floatMaskSize = 16;

FloatMask randomFloatMask()
{
  return FloatMask(floatMaskSize, seed).init(randomBinaryMask(), 0.2, 0.8);
}

std::ostream& operator<<(std::ostream& out, FloatMask const& m)
{
  out << m.cols() << "\n";
  out << m.rows() << "\n";
  out.precision(4);
  out.setf( std::ios::fixed, std:: ios::floatfield );
  for(std::size_t i = 0; i < m.size(); ++i)
  {
    out << m.data()[i] << "\n";
  }
  return out;
}

static auto gen_FloatMask_rand = [](std::ostream& out)
{
  out << randomFloatMask();
};

static auto gen_FloatMask_maskToMoutains = [](std::ostream& out)
{
  out << randomFloatMask().maskToMoutains(1.3f, 0.1f, randomBinaryMask());
};

static auto gen_FloatMask_maskToHeightmap = [](std::ostream& out)
{
  out << randomFloatMask().maskToHeightmap(1.3f, 0.1f, 5, randomBinaryMask());
};

static auto gen_FloatMask_smooth = [](std::ostream& out)
{
  out << randomFloatMask().smooth(1.7f);
};

static auto gen_FloatMask_smooth2 = [](std::ostream& out)
{
  out << randomFloatMask().smooth(1.7f, randomBinaryMask());
};

}
