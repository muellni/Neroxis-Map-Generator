#pragma once

#include <cmath>
#include <algorithm>
#include <optional>

#include <Eigen/Dense>

#include "generator/Random.h"
#include "map/Symmetry.h"

namespace faf
{

class BinaryMask : public Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
{
protected:
  typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> super;
  faf::Random _random;
  Symmetry symmetry = Symmetry::POINT;

public:
  BinaryMask(int size, long seed):
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
  BinaryMask randomize(float density)
  {
    for (std::size_t y = 0; y < rows(); ++y)
    {
      for (std::size_t x = 0; x < cols(); ++x)
      {
        coeffRef(y, x) = _random.nextFloat() < density;
      }
    }
    applySymmetry();
    return *this;
  }

  BinaryMask invert()
  {
    array() = !array();
    return *this;
  }

  BinaryMask enlarge(std::size_t size)
  {
    super result(size, size);
    for (std::size_t y = 0; y < size; ++y)
    {
      auto smallY = std::min(y / (size / std::size_t(rows())), std::size_t(rows()) - 1);
      for (std::size_t x = 0; x < size; ++x)
      {
        auto smallX = std::min(x / (size / std::size_t(cols())), std::size_t(cols()) - 1);
        result(y, x) = (*this)(smallY, smallX);
      }
    }
    super::operator=(result);
    return *this;
  }

  BinaryMask shrink(std::size_t size)
  {
    super result(size, size);
    for (std::size_t y = 0; y < size; ++y)
    {
      auto largeY = (y * rows()) / size + (rows() / size / 2);
      if (largeY >= rows())
        largeY = rows() - 1;
      for (std::size_t x = 0; x < size; ++x)
      {
        auto largeX = (x * cols()) / size + (cols() / size / 2);
        if (largeX >= cols())
          largeX = cols() - 1;
        result(y, x) = (*this)(largeY, largeX);
      }
    }
    super::operator=(result);
    return *this;
  }

protected:
  void deflateRegion(bool inverted, float radius, super& result, int startY, int endY)
  {
    float radius2 = (radius + 0.5f) * (radius + 0.5f);
    for (std::size_t y = startY; y < endY; ++y)
    {
      for (std::size_t x = 0; x < rows(); ++x)
      {
        result(y, x) = !inverted;
        for (int y2 = (int) (y - radius); y2 < y + radius + 1; y2++)
        {
          for (int x2 = (int) (x - radius); x2 < x + radius + 1; x2++)
          {
            if (x2 >= 0 &&
                y2 >= 0 &&
                x2 < rows() &&
                y2 < cols() &&
                (x - x2) * (x - x2) + (y - y2) * (y - y2) <= radius2 &&
                inverted ^ !(*this)(y2, x2))
            {
              result(y, x) = inverted;
              goto loopend;
            }
          }
        }
        loopend: ;
      }
    }
  }
public:
  BinaryMask inflate(float radius)
  {
    return deflate(-radius);
  }

  BinaryMask deflate(float radius)
  {
    super result(rows(), cols());

    bool inverted = radius <= 0;

    deflateRegion(inverted, std::abs(radius), result, 0, (rows() / 4));
    deflateRegion(inverted, std::abs(radius), result, (rows() / 4), (rows() / 2));
    deflateRegion(inverted, std::abs(radius), result, (rows() / 2), (rows() / 4) * 3);
    deflateRegion(inverted, std::abs(radius), result, (rows() / 4) * 3, rows());

    super::operator=(result);
    return *this;
  }

  BinaryMask cutCorners()
  {
    BinaryMask result(*this);
    for (std::size_t y = 0; y < rows(); ++y)
    {
      for (std::size_t x = 0; x < cols(); ++x)
      {
        std::size_t count = 0;
        if (x > 0 && !(*this)(y, x - 1))
        {
          count++;
        }
        if (y > 0 && !(*this)(y - 1, x))
        {
          count++;
        }
        if (x < rows() - 1 && !(*this)(y, x + 1))
        {
          count++;
        }
        if (y < cols() - 1 && !(*this)(y + 1, x))
        {
          count++;
        }
        if (count > 1)
        {
          result(y, x) = false;
        }
      }
    }
    *this = result;
    return *this;
  }

  BinaryMask acid(float strength)
  {
    BinaryMask result(*this);
    for (std::size_t y = 0; y < rows(); ++y)
    {
      for (std::size_t x = 0; x < cols(); ++x)
      {
        if (((x > 0 && !(*this)(y, x - 1)) ||
             (y > 0 && !(*this)(y - 1, x)) ||
             (x < cols() - 1 && !(*this)(y, x + 1)) ||
             (y < rows() - 1 && !(*this)(y + 1, x))) &&
            _random.nextFloat() < strength)
        {
          result(y, x) = false;
        }
      }
    }
    *this = result;
    applySymmetry();
    return *this;
  }

  BinaryMask outline()
  {
    BinaryMask result(*this);
    for (std::size_t y = 0; y < cols(); ++y)
    {
      for (std::size_t x = 0; x < rows(); ++x)
      {
        result(y, x) = ((x > 0 && !(*this)(y, x - 1))
                    || (y > 0 && !(*this)(y - 1, x))
                    || (x < cols() - 1 && !(*this)(y, x + 1))
                    || (y < rows() - 1 && !(*this)(y + 1, x)))
                  && ((x > 0 && (*this)(y, x - 1))
                    || (y > 0 && (*this)(y - 1, x))
                    || (x < cols() - 1 && (*this)(y, x + 1))
                    || (y < rows() - 1 && (*this)(y + 1, x)));
      }
    }
    *this = result;
    applySymmetry();
    return *this;
  }

protected:
  void smoothRegion(float radius, super& result, std::size_t startY, std::size_t endY)
  {
    std::size_t radius2 = (radius + 0.5f) * (radius + 0.5f);
    for (std::size_t y = startY; y < endY; ++y)
    {
      for (std::size_t x = 0; x < cols(); ++x)
      {
        std::size_t count = 0;
        std::size_t count2 = 0;
        for (std::size_t y2 = y - radius; y2 <= y + radius; ++y2)
        {
          for (std::size_t x2 = x - radius; x2 <= x + radius; ++x2)
          {
            if (x2 > 0 &&
                y2 > 0 &&
                x2 < cols() &&
                y2 < rows() &&
                ((x - x2) * (x - x2) + (y - y2) * (y - y2)) <= radius2)
            {
              ++count;
              if ((*this)(y2, x2))
              {
                ++count2;
              }
            }
          }
        }
        if (count2 > count / 2)
        {
          result(y, x) = true;
        }
        else
        {
          result(y, x) = false;
        }
      }
    }
  }

public:
  BinaryMask smooth(float radius)
  {
    super result(rows(), cols());

    smoothRegion(radius, result, 0, (rows() / 4));
    smoothRegion(radius, result, (rows() / 4), (rows() / 2));
    smoothRegion(radius, result, (rows() / 2), (rows() / 4) * 3);
    smoothRegion(radius, result, (rows() / 4) * 3, rows());

    super::operator=(result);
    return *this;
  }

public:
  BinaryMask combine(BinaryMask& other)
  {
    auto size = std::max(rows(), other.rows());
    if (rows() != size)
    {
      enlarge(size);
    }
    if (other.rows() != size)
    {
      other.enlarge(size);
    }

    array() += other.array();
    return *this;
  }

  BinaryMask intersect(BinaryMask& other)
  {
    auto size = std::max(rows(), other.rows());
    if (rows() != size)
    {
      enlarge(size);
    }
    if (other.rows() != size)
    {
      other.enlarge(size);
    }

    array() *= other.array();
    return *this;
  }

  BinaryMask minus(BinaryMask& other)
  {
    auto size = std::max(rows(), other.rows());
    if (rows() != size)
    {
      enlarge(size);
    }
    if (other.rows() != size)
    {
      other.enlarge(size);
    }

    array() *= !other.array();
    return *this;
  }

  BinaryMask fillCircle(float x, float y, float radius, bool value)
  {
    int ex = std::min(int(cols()), int(x + radius));
    int ey = std::min(int(rows()), int(y + radius));
    float dx;
    float dy;
    float radius2 = radius * radius;
    for (int cy = std::max(0, int(y - radius)); cy < ey; ++cy)
    {
      for (int cx = std::max(0, int(x - radius)); cx < ex; ++cx)
      {
        dx = x - cx;
        dy = y - cy;
        if (dx * dx + dy * dy <= radius2)
        {
          (*this)(cy, cx) = value;
        }
      }
    }
    return *this;
  }

  BinaryMask trimEdge(std::size_t rimWidth)
  {
    for (std::size_t a = 0; a < rimWidth; ++a)
    {
      for (std::size_t b = 0; b < rows() - rimWidth; ++b)
      {
        (*this)(b, a) = false;
        (*this)(rows() - 1 - b, cols() - 1 - a) = false;
        (*this)(rows() - 1 - a, b) = false;
        (*this)(a, cols() - 1 - b) = false;
      }
    }
    return *this;
  }

  std::optional<Eigen::Vector2f> getRandomPosition()
  {
    auto cellCount = 0;
    for (std::size_t i = 0; i < size(); ++i)
    {
      if (data()[i])
      {
        ++cellCount;
      }
    }
    if(cellCount == 0)
    {
      return std::optional<Eigen::Vector2f>();
    }
    int cell = _random.nextInt(cellCount) + 1;
    cellCount = 0;
    for (int y = 0; y < rows(); ++y)
    {
      for (int x = 0; x < cols(); ++x)
      {
        if((*this)(y,x))
        {
          ++cellCount;
        }
        if(cellCount == cell)
        {
          return Eigen::Vector2f(x, y);
        }
      }
    }
    return std::optional<Eigen::Vector2f>();
  }

};

}
