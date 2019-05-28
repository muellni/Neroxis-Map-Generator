#pragma once

#include <Eigen/Dense>

#include "generator/Random.h"
#include "map/Symmetry.h"
#include "map/BinaryMask.h"

namespace faf
{

class FloatMask : public Eigen::MatrixXf
{
protected:
  typedef Eigen::MatrixXf super;
  faf::Random _random;
  Symmetry symmetry = Symmetry::POINT;

public:
  FloatMask(int size, long seed):
    super(size, size),
    _random(seed)
  {
  }

protected:
  void applySymmetry()
  {
    switch (symmetry)
    {
      case Symmetry::POINT:
        for (std::size_t y = 0; y < rows() / 2; ++y)
        {
          for (std::size_t x = 0; x < cols(); ++x)
          {
            coeffRef(rows() - y - 1, cols() - x - 1) = coeff(y, x);
          }
        }
        break;
      default:
        break;
    }
  }

public:
  FloatMask init(BinaryMask const& other, float low, float high)
  {
    for (std::size_t y = 0; y < rows(); ++y)
    {
      for (std::size_t x = 0; x < cols(); ++x)
      {
        if (other(y, x))
        {
          coeffRef(y, x) = high;
        }
        else
        {
          coeffRef(y, x) = low;
        }
      }
    }
    return *this;
  }

  FloatMask maskToMoutains(float firstSlope, float slope, BinaryMask const& other)
  {
    BinaryMask otherCopy(other, _random.nextLong());
    FloatMask mountainBase(rows(), 0);
    operator+=(mountainBase.init(otherCopy, 0, firstSlope));
    otherCopy.acid(0.5f);
    for (int i = 0; i < rows(); i++)
    {
      FloatMask layer(rows(), 0);
      operator+=(layer.init(otherCopy, 0, slope));
      otherCopy.acid(0.5f);
    }
    applySymmetry();
    return *this;
  }

  FloatMask maskToHeightmap(float slope, float underWaterSlope, int maxRepeat, BinaryMask const& other)
  {
    BinaryMask otherCopy(other, _random.nextLong());
    for (int i = 0; i < rows(); ++i)
    {
      FloatMask layer(rows(), 0);
      operator+=(layer.init(otherCopy, 0, slope));
      otherCopy.acid(0.5f);
    }
    otherCopy = BinaryMask(other, _random.nextLong());
    otherCopy.invert();
    for (int i = 0; i < maxRepeat; ++i)
    {
      FloatMask layer(rows(), 0);
      operator+=(layer.init(otherCopy, 0 , -underWaterSlope));
      otherCopy.acid(0.5f);
    }
    return *this;
  }

  FloatMask smooth(float radius)
  {
    super result(rows(), cols());

    smoothRegion(radius, result, 0, (rows() / 4));
    smoothRegion(radius, result, (rows() / 4), (rows() / 2));
    smoothRegion(radius, result, (rows() / 2), (rows() / 4) * 3);
    smoothRegion(radius, result, (rows() / 4) * 3, rows());

    super::operator=(result);
    return *this;
  }

protected:
  void smoothRegion(float radius, super& result, int startY, int endY)
  {
    float radius2 = (radius + 0.5f) * (radius + 0.5f);
    for (std::size_t y = startY; y < endY; ++y)
    {
      for (std::size_t x = 0; x < cols(); ++x)
      {
        int count = 0;
        float avg = 0;
        for (int y2 = (y - radius); y2 <= y + radius; ++y2)
        {
          for (int x2 = (x - radius); x2 <= x + radius; ++x2)
          {
            if (x2 > 0 && y2 > 0 && x2 < cols() && y2 < rows() && (x - x2) * (x - x2) + (y - y2) * (y - y2) <= radius2)
            {
              count++;
              avg += coeff(y2, x2);
            }
          }
        }
        result(y, x) = avg / count;
      }
    }
  }

public:
  FloatMask smooth(float radius, BinaryMask const& limiter)
  {
    super result(rows(), cols());

    smoothRegion(radius, limiter, result, 0, (rows() / 4));
    smoothRegion(radius, limiter, result, (rows() / 4), (rows() / 2));
    smoothRegion(radius, limiter, result, (rows() / 2), (rows() / 4) * 3);
    smoothRegion(radius, limiter, result, (rows() / 4) * 3, rows());

    super::operator=(result);
    return *this;
  }

protected:
  void smoothRegion(float radius, BinaryMask const& limiter, super& result, int startY, int endY) {
    float radius2 = (radius + 0.5f) * (radius + 0.5f);
    for (std::size_t y = startY; y < endY; ++y)
    {
      for (std::size_t x = 0; x < cols(); ++x)
      {
        if(limiter(y, x))
        {
          int count = 0;
          float avg = 0;
          for (int y2 = (int) (y - radius); y2 <= y + radius; ++y2)
          {
            for (int x2 = (int) (x - radius); x2 <= x + radius; ++x2)
            {
              if (x2 > 0 && y2 > 0 && x2 < cols() && y2 < rows() && (x - x2) * (x - x2) + (y - y2) * (y - y2) <= radius2)
              {
                count++;
                avg += coeff(y2, x2);
              }
            }
          }
          result(y, x) = avg / count;
        }
        else
        {
          result(y, x) = coeff(y, x);
        }
      }
    }
  }
};

}
