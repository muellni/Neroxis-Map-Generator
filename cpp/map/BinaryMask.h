#include <random>
#include <cmath>
#include <map/Symmetry.h>
#include <Eigen/Dense>


class BinaryMask : public Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>
{
protected:
  typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> super;
  std::default_random_engine randomGenerator;
  std::uniform_real_distribution<float> randFloat;
  Symmetry symmetry = Symmetry::POINT;


public:
  BinaryMask(int size, long seed):
    super(size, size),
    randomGenerator(seed),
    randFloat(0.0, 1.0)
  {
  }

protected:
  void applySymmetry()
  {
    switch (symmetry)
    {
    case Symmetry::POINT:
      for (std::size_t y = 0; y < cols() / 2; ++y)
      {
        for (std::size_t x = 0; x < rows(); ++x)
        {
          operator()(rows() - x - 1, cols() - y - 1) = operator()(x, y);
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
    for (std::size_t x = 0; x < rows(); ++x)
    {
      for (std::size_t y = 0; y < cols(); ++y)
      {
        operator()(x,y) = randFloat(randomGenerator) < density;
      }
    }
    return *this;
  }

  BinaryMask invert()
  {
    array() = !array();
    return *this;
  }

  BinaryMask enlarge(std::size_t size)
  {
    conservativeResize(size, size);
    return *this;
  }

  BinaryMask shrink(std::size_t size)
  {
    conservativeResize(size, size);
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
        result(x,y) = !inverted;
        for (int y2 = (int) (y - radius); y2 < y + radius + 1; y2++)
        {
          for (int x2 = (int) (x - radius); x2 < x + radius + 1; x2++)
          {
            if (x2 >= 0 &&
                y2 >= 0 &&
                x2 < rows() &&
                y2 < cols() &&
                (x - x2) * (x - x2) + (y - y2) * (y - y2) <= radius2 &&
                inverted ^ !operator()(x2,y2))
            {
              result(x,y) = inverted;
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

    deflateRegion(inverted, std::abs(radius), result, 0, (cols() / 4));
    deflateRegion(inverted, std::abs(radius), result, (rows() / 4), (cols() / 2));
    deflateRegion(inverted, std::abs(radius), result, (rows() / 2), (cols() / 4) * 3);
    deflateRegion(inverted, std::abs(radius), result, (rows() / 4) * 3, cols());

    super::operator=(result);
    return *this;
  }

  BinaryMask cutCorners()
  {
    BinaryMask result(*this);
    for (std::size_t y = 0; y < cols(); ++y)
    {
      for (std::size_t x = 0; x < rows(); ++x)
      {
        std::size_t count = 0;
        if (x > 0 && !(*this)(x - 1, y))
        {
          count++;
        }
        if (y > 0 && !(*this)(x, y - 1))
        {
          count++;
        }
        if (x < rows() - 1 && !(*this)(x + 1, y))
        {
          count++;
        }
        if (y < cols() - 1 && !(*this)(x, y + 1))
        {
          count++;
        }
        if (count > 1)
        {
          result(x, y) = false;
        }
      }
    }
    *this = result;
    return *this;
  }

  BinaryMask acid(float strength)
  {
    BinaryMask result(*this);
    for (std::size_t y = 0; y < cols(); ++y)
    {
      for (std::size_t x = 0; x < rows(); ++x)
      {
        if (((x > 0 && !(*this)(x - 1,y)) ||
             (y > 0 && !(*this)(x, y - 1)) ||
             (x < rows() - 1 && !(*this)(x + 1, y)) ||
             (y < cols() - 1 && !(*this)(x, y + 1))) &&
            randFloat(randomGenerator) < strength)
        {
          result(x,y) = false;
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
        result(x, y) = ((x > 0 && !(*this)(x - 1, y))
                    || (y > 0 && !(*this)(x, y - 1))
                    || (x < rows() - 1 && !(*this)(x + 1, y))
                    || (y < cols() - 1 && !(*this)(x, y + 1)))
                  && ((x > 0 && (*this)(x - 1, y))
                    || (y > 0 && (*this)(x, y - 1))
                    || (x < rows() - 1 && (*this)(x + 1, y))
                    || (y < cols() - 1 && (*this)(x, y + 1)));
      }
    }
    *this = result;
    applySymmetry();
    return *this;
  }

#if 0


	public BinaryMask smooth(float radius) {
		boolean[][] maskCopy = new boolean[getSize()][getSize()];

		Thread[] threads = new Thread[4];
		threads[0] = new Thread(() -> smoothRegion(radius, maskCopy, 0, (getSize() / 4)));
		threads[1] = new Thread(() -> smoothRegion(radius, maskCopy, (getSize() / 4), (getSize() / 2)));
		threads[2] = new Thread(() -> smoothRegion(radius, maskCopy, (getSize() / 2), (getSize() / 4) * 3));
		threads[3] = new Thread(() -> smoothRegion(radius, maskCopy, (getSize() / 4) * 3, getSize()));

		Arrays.stream(threads).forEach(Thread::start);
		for (Thread f : threads) {
			try {
				f.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		mask = maskCopy;
		return this;
	}

	private void smoothRegion(float radius, boolean[][] maskCopy, int startY, int endY) {
		float radius2 = (radius + 0.5f) * (radius + 0.5f);
		for (int y = startY; y < endY; y++) {
			for (int x = 0; x < getSize(); x++) {
				int count = 0;
				int count2 = 0;
				for (int y2 = (int) (y - radius); y2 <= y + radius; y2++) {
					for (int x2 = (int) (x - radius); x2 <= x + radius; x2++) {
						if (x2 > 0 && y2 > 0 && x2 < getSize() && y2 < getSize() && (x - x2) * (x - x2) + (y - y2) * (y - y2) <= radius2) {
							count++;
							if (mask[x2][y2])
								count2++;
						}
					}
				}
				if (count2 > count / 2) {
					maskCopy[x][y] = true;
				}
			}
		}
	}

	public BinaryMask combine(BinaryMask other) {
		int size = Math.max(getSize(), other.getSize());
		if (getSize() != size)
			enlarge(size);
		if (other.getSize() != size)
			other.enlarge(size);
		boolean[][] maskCopy = new boolean[getSize()][getSize()];
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				maskCopy[x][y] = get(x, y) || other.get(x, y);
			}
		}
		mask = maskCopy;
		return this;
	}

	public BinaryMask intersect(BinaryMask other) {
		int size = Math.max(getSize(), other.getSize());
		if (getSize() != size)
			enlarge(size);
		if (other.getSize() != size)
			other.enlarge(size);
		boolean[][] maskCopy = new boolean[getSize()][getSize()];
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				maskCopy[x][y] = get(x, y) && other.get(x, y);
			}
		}
		mask = maskCopy;
		return this;
	}

	public BinaryMask minus(BinaryMask other) {
		int size = Math.max(getSize(), other.getSize());
		if (getSize() != size)
			enlarge(size);
		if (other.getSize() != size)
			other.enlarge(size);
		boolean[][] maskCopy = new boolean[getSize()][getSize()];
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				maskCopy[x][y] = get(x, y) && !other.get(x, y);
			}
		}
		mask = maskCopy;
		return this;
	}

	public BinaryMask fillCircle(float x, float y, float radius, boolean value) {
		int ex = (int) StrictMath.min(getSize(), x + radius);
		int ey = (int) StrictMath.min(getSize(), y + radius);
		float dx;
		float dy;
		float radius2 = radius * radius;
		for (int cy = (int) StrictMath.max(0, y - radius); cy < ey; cy++) {
			for (int cx = (int) StrictMath.max(0, x - radius); cx < ex; cx++) {
				dx = x - cx;
				dy = y - cy;
				if (dx * dx + dy * dy <= radius2) {
					mask[cx][cy] = value;
				}
			}
		}
		return this;
	}
	
	public BinaryMask trimEdge(int rimWidth) {
		for (int a = 0; a < rimWidth; a++) {
			for (int b = 0; b < getSize() - rimWidth; b++) {
				mask[a][b] = false;
				mask[getSize() - 1 - a][getSize() - 1 - b] = false;
				mask[b][getSize() - 1 - a] = false;
				mask[getSize() - 1 - b][a] = false;
			}
		}
		return this;
	}
	
	public Vector2f getRandomPosition(){
		int cellCount = 0;
		for (int y = 0; y < getSize(); y++) {
			for (int x = 0; x < getSize(); x++) {
				if(mask[x][y])
					cellCount++;
			}
		}
		if(cellCount == 0)
			return null;
		int cell = random.nextInt(cellCount) + 1;
		cellCount = 0;
		for (int y = 0; y < getSize(); y++) {
			for (int x = 0; x < getSize(); x++) {
				if(mask[x][y])
					cellCount++;
				if(cellCount == cell)
					return new Vector2f(x,y);
			}
		}
		return null;
	}

#endif
};
