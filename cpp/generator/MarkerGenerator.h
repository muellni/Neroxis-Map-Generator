#pragma once

#include <cmath>
#include <Eigen/Dense>

#include "generator/Random.h"
#include "map/SCMap.h"

namespace faf {

class MarkerGenerator {

  SCMap map;
  Random _random;

public:
  MarkerGenerator(SCMap const& map, long seed):
    map(map),
    _random(seed)
  {
  }

protected:
  void placeOnHeightmap(float x, float z, Eigen::Vector3f& v)
  {
    v.x() = x;
    v.z() = z;
    v.y() = map.heightmap.at<uint16_t>(x, z) * map.HEIGHTMAP_SCALE;
  }

  Eigen::Vector3f placeOnHeightmap(float x, float z)
  {
    Eigen::Vector3f v = {x, 0, z};
    v.y() = map.heightmap.at<uint16_t>(x, z) * map.HEIGHTMAP_SCALE;
    return v;
  }

  void randomizeVectorPair(Eigen::Vector3f& v1, Eigen::Vector3f& v2)
  {
    placeOnHeightmap(static_cast<int>(_random.nextFloat() * map.size),
                     static_cast<int>(_random.nextFloat() * map.size),
                     v1);
    placeOnHeightmap(map.size - v1.x(), map.size - v1.z(), v2);
  }

public:
  void generateSpawns(BinaryMask const& spawnable, float separation)
  {
    BinaryMask spawnableCopy(spawnable, _random.nextLong());
    auto location = spawnableCopy.getRandomPosition();
    for (int i = 0; i < (map.spawns.size()-1); i += 2)
    {
      if (location)
      {
        spawnableCopy.fillCircle(location->x(), location->y(), separation, false);
        spawnableCopy.fillCircle(map.size - location->x(), map.size - location->y(), separation, false);
        map.spawns[i] = placeOnHeightmap(location->x(), location->y());
        map.spawns[i + 1] = placeOnHeightmap(map.size - location->x(), map.size - location->y());
        location = spawnableCopy.getRandomPosition();
      }
    }
  }

  void generateMexs(BinaryMask const& spawnable)
  {
    int baseMexCount = map.spawns.size() * 4;

    for (int i = 0; i < map.spawns.size(); ++i)
    {
      map.mexs[i * 4] = Eigen::Vector3f::Zero();
      map.mexs[i * 4 + 1] = Eigen::Vector3f::Zero();;
      map.mexs[i * 4 + 2] = Eigen::Vector3f::Zero();;
      map.mexs[i * 4 + 3] = Eigen::Vector3f::Zero();;
      placeOnHeightmap(map.spawns[i].x() + 10, map.spawns[i].z(), map.mexs[i * 4]);
      placeOnHeightmap(map.spawns[i].x() - 10, map.spawns[i].z(), map.mexs[i * 4 + 1]);
      placeOnHeightmap(map.spawns[i].x(), map.spawns[i].z() + 10, map.mexs[i * 4 + 2]);
      placeOnHeightmap(map.spawns[i].x(), map.spawns[i].z() - 10, map.mexs[i * 4 + 3]);
    }

    for (int i = baseMexCount; i < (map.mexs.size()-1); i+= 2)
    {
      map.mexs[i] = Eigen::Vector3f::Zero();
      map.mexs[i + 1] = Eigen::Vector3f::Zero();
      randomizeVectorPair(map.mexs[i], map.mexs[i + 1]);
    }

    for (int i = baseMexCount; i < (map.mexs.size()-1); i+= 2)
    {
      while (!isMexValid(i, 16, 16, spawnable))
      {
        randomizeVectorPair(map.mexs[i], map.mexs[i + 1]);
      }
    }
  }

  void generateHydros(BinaryMask const& spawnable)
  {
    int baseHydroCount = map.spawns.size();

    for (int i = 0; i < map.spawns.size(); ++i)
    {
      int dx = map.spawns[i].x() < map.size / 2 ? -4 : +4;
      int dz = map.spawns[i].z() < map.size / 2 ? -14 : +14;
      map.hydros[i] = Eigen::Vector3f::Zero();
      placeOnHeightmap(map.spawns[i].x() + dx, map.spawns[i].z() + dz, map.hydros[i]);
    }

    for (int i = baseHydroCount; i < map.hydros.size() - 1; i+= 2)
    {
      map.hydros[i] = Eigen::Vector3f::Zero();
      map.hydros[i + 1] = Eigen::Vector3f::Zero();
      randomizeVectorPair(map.hydros[i], map.hydros[i + 1]);
    }

    for (int i = baseHydroCount; i < (map.hydros.size() - 1); i+= 2) {
      while (!isHydroValid(i, 64, 16, 32, spawnable))
      {
        randomizeVectorPair(map.hydros[i], map.hydros[i + 1]);
      }
    }
  }

protected:
  bool isMexValid(int index, float distance, float edgeSpacing, BinaryMask const& spawnable)
  {
    bool valid = true;
    float dx;
    float dz;
    if (map.mexs[index].x() < edgeSpacing)
      valid = false;
    if (map.mexs[index].z() < edgeSpacing)
      valid = false;
    if (map.mexs[index].x() > map.size - edgeSpacing)
      valid = false;
    if (map.mexs[index].z() > map.size - edgeSpacing)
      valid = false;

    for (int i = 0; i < map.mexs.size(); ++i)
    {
      if (i != index)
      {
        dx = map.mexs[i].x() - map.mexs[index].x();
        dz = map.mexs[i].z() - map.mexs[index].z();
        if (std::sqrt(dx * dx + dz * dz) < distance)
          valid = false;
      }
    }
    if (!spawnable(static_cast<int>(map.mexs[index].z()),
                   static_cast<int>(map.mexs[index].x())))
      valid = false;
    return valid;
  }

  bool isHydroValid(int index, float hydroDistance, float mexDistance, float edgeSpacing, BinaryMask const& spawnable)
  {
    bool valid = true;
    float dx;
    float dz;
    if (map.hydros[index].x() < edgeSpacing)
      valid = false;
    if (map.hydros[index].z() < edgeSpacing)
      valid = false;
    if (map.hydros[index].x() > map.size - edgeSpacing)
      valid = false;
    if (map.hydros[index].z() > map.size - edgeSpacing)
      valid = false;

    for (int i = 0; i < map.hydros.size(); ++i)
    {
      if (i != index)
      {
        dx = map.hydros[i].x() - map.hydros[index].x();
        dz = map.hydros[i].z() - map.hydros[index].z();
        if (std::sqrt(dx * dx + dz * dz) < hydroDistance)
          valid = false;
      }
    }
    for (int i = 0; i < map.mexs.size(); ++i)
    {
      if (i != index)
      {
        dx = map.hydros[i].x() - map.hydros[index].x();
        dz = map.hydros[i].z() - map.hydros[index].z();
        if (std::sqrt(dx * dx + dz * dz) < mexDistance)
          valid = false;
      }
    }
    if (!spawnable(static_cast<int>(map.hydros[index].z()), static_cast<int>(map.hydros[index].x())))
      valid = false;
    return valid;
  }
};

}
