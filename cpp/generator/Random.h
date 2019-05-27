#pragma once

#include <cstdint>
// nextLong
// nextFloat
// nextInt
namespace faf
{

class Random
{
protected:
    int64_t seed;
    static constexpr int64_t multiplier = 0x5DEECE66DL;

    static constexpr int64_t addend = 0xBL;

    static constexpr int64_t mask()
    {
      return (1L << 48) - 1;
    }

    static constexpr double DOUBLE_UNIT = 0x1.0p-53; // 1.0 / (1L << 53)

public:
    Random(int64_t seed)
    {
      setSeed(seed);
    }

    static int64_t initialScramble(int64_t seed)
    {
        return (seed ^ multiplier) & mask();
    }

    void setSeed(int64_t seed)
    {
        this->seed = initialScramble(seed);
        haveNextNextGaussian = false;
    }

    protected:

    int32_t next(int32_t bits)
    {
        int64_t oldseed, nextseed;
        oldseed = seed;
        nextseed = (oldseed * multiplier + addend) & mask();
        seed = nextseed;
        return (uint64_t)nextseed >> (48 - bits);
    }

public:
    int32_t nextInt()
    {
        return next(32);
    }

    int32_t nextInt(int32_t bound)
    {
        int32_t r = next(31);
        int32_t m = bound - 1;
        if ((bound & m) == 0)  // i.e., bound is a power of 2
            r = (int32_t)((bound * (long)r) >> 31);
        else {
            for (int32_t u = r;
                 u - (r = u % bound) + m < 0;
                 u = next(31))
                ;
        }
        return r;

    }

    int64_t nextLong()
    {
        return ((int64_t)(next(32)) << 32) + next(32);
    }

    bool nextBoolean()
    {
        return next(1) != 0;
    }

    float nextFloat()
    {
        return next(24) / ((float)(1 << 24));
    }

    double nextDouble()
    {
        return (((int64_t)(next(26)) << 27) + next(27)) * DOUBLE_UNIT;
    }

    protected:
    bool haveNextNextGaussian = false;
};

}
