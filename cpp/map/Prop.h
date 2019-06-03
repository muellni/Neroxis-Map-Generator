#pragma once

#include <string>
#include <Eigen/Dense>

namespace faf {

struct Prop
{
  std::string path;
  Eigen::Vector3f position;
  float rotation;
};

}
