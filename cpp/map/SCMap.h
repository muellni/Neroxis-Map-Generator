#pragma once

#include <string>

#include <Eigen/Dense>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "map/Prop.h"
#include "map/FloatMask.h"

namespace faf {

struct SCMap
{
  static constexpr int SIGNATURE = 443572557;
  static constexpr int VERSION_MAJOR = 2;
  static constexpr int VERSION_MINOR = 56;

  static constexpr float HEIGHTMAP_SCALE = 1.f / 128.f;
  inline static const std::string TERRAIN_SHADER_PATH = "TTerrain";
  inline static const std::string BACKGROUND_PATH = "/textures/environment/defaultbackground.dds";
  inline static const std::string SKYCUBE_PATH = "/textures/environment/defaultskycube.dds";
  inline static const std::string CUBEMAP_NAME = "<default>";
  inline static const std::string CUBEMAP_PATH = "/textures/environment/defaultenvcube.dds";
  static constexpr float LIGHTING_MULTIPLIER = 1.5f;
  inline static const Eigen::Vector3f SUN_DIRECTION = {0.707f, 0.707f, 0.f};
  inline static const Eigen::Vector3f SUN_AMBIANCE_COLOR = {0.2f, 0.2f, 0.2f};
  inline static const Eigen::Vector3f SUN_COLOR = {1.0f, 1.0f, 1.0f};
  inline static const Eigen::Vector3f SHADOW_COLOR = {0.7f, 0.7f, 0.75f};
  inline static const Eigen::Vector4f SPECULAR_COLOR = {0.0f, 0.0f, 0.0f, 0.0f};
  static constexpr float BLOOM = 0.08f;
  inline static const Eigen::Vector3f FOG_COLOR = {1.0f, 1.0f, 1.0f};
  static constexpr float FOG_START = 0.f;
  static constexpr float FOG_END = 1000.f;

  static constexpr float WATER_HEIGHT = 25.f;
  static constexpr float WATER_DEEP_HEIGHT = 20.f;
  static constexpr float WATER_ABYSS_HEIGHT = 10.f;
  inline static const Eigen::Vector3f WATER_SURFACE_COLOR = {0.0f, 0.7f, 1.5f};
  inline static const Eigen::Vector2f WATER_COLOR_LERP = {0.064f, 0.119f};
  static constexpr float WATER_REFRACTION = 0.375f;
  static constexpr float WATER_FRESNEL_BIAS = 0.15f;
  static constexpr float WATER_FRESNEL_POWER = 1.5f;
  static constexpr float WATER_UNIT_REFLECTION = 0.5f;
  static constexpr float WATER_SKY_REFLECTION = 1.5f;
  static constexpr float WATER_SUN_SHININESS = 50.f;
  static constexpr float WATER_SUN_STRENGH = 10.f;
  inline static const Eigen::Vector3f WATER_SUN_DIRECTION = {0.09954818f, -0.9626309f, 0.2518569f}; // why != SUN_DIRECTION
  inline static const Eigen::Vector3f WATER_SUN_COLOR = {0.81274265f, 0.47409984f, 0.33864275f};
  static constexpr float WATER_SUN_REFLECTION = 5.f;
  static constexpr float WATER_SUN_GLOW = 0.1f;
  inline static const std::string WATER_CUBEMAP_PATH = "/textures/engine/waterCubemap.dds";
  inline static const std::string WATER_RAMP_PATH = "/textures/engine/waterramp.dds";

  static constexpr int WAVE_NORMAL_COUNT = 4;
  static constexpr float WAVE_NORMAL_REPEATS[] = { 0.0009f, 0.009f, 0.05f, 0.5f };
  inline static const Eigen::Vector2f WAVE_NORMAL_MOVEMENTS[] = { {0.5f, -0.95f}, {0.05f, -0.095f}, {0.01f, 0.03f}, {0.0005f, 0.0009f} };
  inline static const std::string WAVE_TEXTURE_PATHS[] = { "/textures/engine/waves.dds", "/textures/engine/waves.dds", "/textures/engine/waves.dds", "/textures/engine/waves.dds" }; // always same?

  static constexpr int TERRAIN_TEXTURE_COUNT = 10;
  inline static const std::string TERRAIN_TEXTURE_PATHS[] = {
      "/env/evergreen/layers/SandLight_albedo.dds",
      "/env/evergreen/layers/grass001_albedo.dds",
      "/env/evergreen/layers/Dirt001_albedo.dds",
      "/env/evergreen/layers/RockMed_albedo.dds",
      "/env/evergreen/layers/snow001_albedo.dds",
      "",
      "",
      "",
      "",
      "/env/evergreen/layers/macrotexture000_albedo.dds" };
  static constexpr float TERRAIN_TEXTURE_SCALES[] = { 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 128.f };
  static constexpr int TERRAIN_NORMALS_COUNT = 9;
  inline static const std::string TERRAIN_NORMALS_PATHS[] = {
      "/env/evergreen/layers/SandLight_normals.dds",
      "/env/evergreen/layers/grass001_normals.dds",
      "/env/evergreen/layers/Dirt001_normals.dds",
      "/env/evergreen/layers/RockMed_normals.dds",
      "/env/evergreen/layers/snow001_normals.dds",
      "",
      "",
      "",
      "", };
  static constexpr float TERRAIN_NORMALS_SCALES[] = { 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f, 4.f };

  int size; // must be a power of 2. 512 equals a 10x10km Map

  std::vector<Eigen::Vector3f> spawns;
  std::vector<Eigen::Vector3f> mexs;
  std::vector<Eigen::Vector3f> hydros;
  std::vector<Prop> props;

  cv::Mat preview;
  cv::Mat heightmap;
  cv::Mat normalMap;
  cv::Mat textureMasksLow;
  cv::Mat textureMasksHigh;

  cv::Mat waterMap;
  cv::Mat waterFoamMask;
  cv::Mat waterFlatnessMask;
  cv::Mat waterDepthBiasMask;
  cv::Mat terrainType;

  SCMap(int size, int spawnCount, int mexCount, int hydroCount):
    size(size)
  {
    spawns.resize(spawnCount);
    mexs.resize(mexCount);
    hydros.resize(hydroCount);

    preview = cv::Mat(256, 256, CV_8UC4, cv::Scalar(0));
    heightmap = cv::Mat(size + 1, size + 1, CV_16UC1, cv::Scalar(0));
    normalMap = cv::Mat(size / 2, size / 2, CV_8UC4, cv::Scalar(0));
    textureMasksLow = cv::Mat(size / 2, size / 2, CV_8UC4, cv::Scalar(0));
    textureMasksHigh = cv::Mat(size / 2, size / 2, CV_8UC4, cv::Scalar(0));

    waterMap = cv::Mat(size / 2, size / 2, CV_8UC1, cv::Scalar(0));
    waterFoamMask = cv::Mat(size / 2, size / 2, CV_8UC1, cv::Scalar(0));
    waterFlatnessMask = cv::Mat(size / 2, size / 2, CV_8UC1, cv::Scalar(0));
    waterFlatnessMask = cv::Scalar(255);
    waterDepthBiasMask = cv::Mat(size / 2, size / 2, CV_8UC1, cv::Scalar(0));
    terrainType = cv::Mat(size / 2, size / 2, CV_8UC4, cv::Scalar(0));
  }

  void setHeightmap(FloatMask const& heightmap)
  {
    for (int y = 0; y < size + 1; ++y)
    {
      for (int x = 0; x < size + 1; ++x)
      {
        this->heightmap.at<uint16_t>(x, y) = heightmap(y, x) / HEIGHTMAP_SCALE;
      }
    }
  }

  void setTextureMaskLow(FloatMask const& mask0, FloatMask const& mask1, FloatMask const& mask2, FloatMask const& mask3)
  {
    for (int y = 0; y < size / 2; ++y)
    {
      for (int x = 0; x < size / 2; ++x)
      {
        textureMasksLow.at<cv::Vec4b>(x, y) = { static_cast<uint8_t>(mask0(y, x) * 256.f),
                                                static_cast<uint8_t>(mask1(y, x) * 256.f),
                                                static_cast<uint8_t>(mask2(x, y) * 256.f),
                                                static_cast<uint8_t>(mask3(x, y) * 256.f) };
      }
    }
  }
};

}
