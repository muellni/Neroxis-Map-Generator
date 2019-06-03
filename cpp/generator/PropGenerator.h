#pragma once

#include <string>

#include <Eigen/Dense>

#include "generator/Random.h"
#include "map/SCMap.h"

namespace faf {

struct PropGenerator {
  inline static std::vector<std::string> TREE_GROUPS()
  {
    return {
      "/env/evergreen/props/trees/groups/Brch01_Group01_prop.bp",
      "/env/evergreen/props/trees/groups/Brch01_Group02_prop.bp",
      "/env/evergreen/props/trees/groups/Pine06_GroupA_prop.bp",
      "/env/evergreen/props/trees/groups/Pine06_GroupB_prop.bp",
      "/env/evergreen/props/trees/groups/Pine07_GroupA_prop.bp",
      "/env/evergreen/props/trees/groups/Pine07_GroupB_prop.bp" };
  };

  inline static std::vector<std::string> ROCKS()
  {
    return {
      "/env/evergreen/props/rocks/Rock01_prop.bp",
      "/env/evergreen/props/rocks/Rock02_prop.bp",
      "/env/evergreen/props/rocks/Rock03_prop.bp",
      "/env/evergreen/props/rocks/Rock04_prop.bp",
      "/env/evergreen/props/rocks/Rock05_prop.bp"};
  };

  inline static std::vector<std::string> FIELD_STONES()
  {
    return {
      "/env/evergreen/props/rocks/fieldstone01_prop.bp",
      "/env/evergreen/props/rocks/fieldstone02_prop.bp",
      "/env/evergreen/props/rocks/fieldstone03_prop.bp",
      "/env/evergreen/props/rocks/fieldstone04_prop.bp"};
  };

  SCMap map;
protected:
  Random _random;

public:
  PropGenerator(SCMap const& map, long seed) :
    map(map),
    _random(seed)
  {
  }

protected:
  Eigen::Vector3f placeOnHeightmap(float x, float z, Eigen::Vector3f& v)
  {
    v.x() = x;
    v.z() = z;
    v.y() = map.heightmap.at<uint16_t>(x, z) * map.HEIGHTMAP_SCALE;
    return v;
  }

  Eigen::Vector3f placeOnHeightmap(float x, float z)
  {
    Eigen::Vector3f v = {x, 0, z};
    v.y() = map.heightmap.at<uint16_t>(x, z) * map.HEIGHTMAP_SCALE;
    return v;
  }

public:
  void generateProps(BinaryMask const& spawnable, std::vector<std::string> const& paths, float separation)
  {
    BinaryMask spawnableCopy(spawnable, _random.nextLong());
    auto location = spawnableCopy.getRandomPosition();
    while (location)
    {
      spawnableCopy.fillCircle(location->x(), location->y(), separation, false);
      spawnableCopy.fillCircle(map.size - location->x(), map.size - location->y(), separation, false);
      Prop prop1 = {paths[_random.nextInt(paths.size())],
                    placeOnHeightmap(location->x(), location->y()),
                    _random.nextFloat()};
      Prop prop2 = {prop1.path,
                    placeOnHeightmap(map.size - location->x(), map.size - location->y()),
                    prop1.rotation + 0.5f};
      map.props.push_back(prop1);
      map.props.push_back(prop2);
      location = spawnableCopy.getRandomPosition();
    }
  }
};

}
