#pragma once

#include "map/BinaryMask.h"
#include "test/config.h"

namespace faf {

static constexpr std::size_t binaryMaskSize = 16;

static constexpr float binaryMaskDensity = 0.5f;

BinaryMask randomBinaryMask()
{
  return BinaryMask(binaryMaskSize, seed).randomize(binaryMaskDensity);
}

static auto gen_BinaryMask_rand = [](std::ostream& out)
{
  out << randomBinaryMask() << "\n";
};

static auto gen_BinaryMask_invert = [](std::ostream& out)
{
  out << randomBinaryMask().invert() << "\n";
};

static auto gen_BinaryMask_enlarge = [](std::ostream& out)
{
  out << randomBinaryMask().enlarge(binaryMaskSize + 16) << "\n";
};

static auto gen_BinaryMask_shrink = [](std::ostream& out)
{
  out << randomBinaryMask().shrink(binaryMaskSize - 5) << "\n";
};

static auto gen_BinaryMask_inflate = [](std::ostream& out)
{
  out << randomBinaryMask().inflate(0.5f) << "\n";
};

static auto gen_BinaryMask_deflate = [](std::ostream& out)
{
  out << randomBinaryMask().deflate(0.5f) << "\n";
};

static auto gen_BinaryMask_cutCorners = [](std::ostream& out)
{
  out << randomBinaryMask().cutCorners() << "\n";
};

static auto gen_BinaryMask_acid = [](std::ostream& out)
{
  out << randomBinaryMask().acid(0.4) << "\n";
};

static auto gen_BinaryMask_outline = [](std::ostream& out)
{
  out << randomBinaryMask().outline() << "\n";
};

static auto gen_BinaryMask_smooth = [](std::ostream& out)
{
  out << randomBinaryMask().smooth(0.4) << "\n";
};

static auto gen_BinaryMask_combine = [](std::ostream& out)
{
  auto other = randomBinaryMask().acid(0.4);
  out << randomBinaryMask().combine(other) << "\n";
};

static auto gen_BinaryMask_intersect = [](std::ostream& out)
{
  auto other = randomBinaryMask().acid(0.4);
  out << randomBinaryMask().intersect(other) << "\n";
};

static auto gen_BinaryMask_minus = [](std::ostream& out)
{
  auto other = randomBinaryMask().acid(0.4);
  out << randomBinaryMask().minus(other) << "\n";
};

static auto gen_BinaryMask_fillCircle = [](std::ostream& out)
{
  out << randomBinaryMask().fillCircle(8.3, 12.7, 3.2, true) << "\n";
};

static auto gen_BinaryMask_trimEdge = [](std::ostream& out)
{
  out << randomBinaryMask().trimEdge(5) << "\n";
};

static auto gen_BinaryMask_getRandomPosition = [](std::ostream& out)
{
  auto pos = randomBinaryMask().getRandomPosition();
  if (pos)
  out << *pos << "\n";
};

}
