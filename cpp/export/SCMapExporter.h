#pragma once

#include <fstream>
#include <cmath>
#include <filesystem>

#include "map/SCMap.h"

namespace faf {

struct SCMapExporter
{
protected:
  static constexpr char DDS_HEADER_1[] = { 68, 68, 83, 32, 124, 0, 0, 0, 7, 16, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 65, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1,
      2, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  static constexpr char DDS_HEADER_2[] = { 68, 68, 83, 32, 124, 0, 0, 0, 7, 16, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 4, 0, 0, 0, 68, 88, 84, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  static constexpr char DDS_HEADER_3[] = { 68, 68, 83, 32, 124, 0, 0, 0, 7, 16, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 4, 0, 0, 0, 68, 88, 84, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };


public:
  static void exportSCMAP(std::filesystem::path const& folderPath, std::string const& mapname, SCMap const& map)
  {
    std::ofstream out;
    out.open((folderPath / mapname / (mapname + ".scmap")).string());
    exportSCMAP(out, map);
  }

  static void exportSCMAP(std::ostream& out, SCMap const& map)
  {
    // header
    out << writeInt(map.SIGNATURE);
    out << writeInt(map.VERSION_MAJOR);
    out << writeInt(-1091567891); // unknown
    out << writeInt(2); // unknown
    out << writeFloat(map.size); // width
    out << writeFloat(map.size); // height
    out << writeInt(0); // unknown
    out << writeInt<uint16_t>(0); // unknown
    // 40080
    out << writeInt<int32_t>(sizeof(DDS_HEADER_1) + map.preview.total() * map.preview.elemSize()); // preview image byte count 8000 0400
    out.write(DDS_HEADER_1, sizeof(DDS_HEADER_1));
    out.write(reinterpret_cast<const char *>(map.preview.ptr()), map.preview.total() * map.preview.elemSize()); // preview image data
    out << writeInt(map.VERSION_MINOR);

    // heightmap
    out << writeInt(map.size); // width
    out << writeInt(map.size); // height
    out << writeFloat(map.HEIGHTMAP_SCALE);
    out.write(reinterpret_cast<const char *>(map.heightmap.ptr()), map.heightmap.total() * map.heightmap.elemSize()); // heightmap data
    out << writeInt<uint8_t>(0); // unknown

    // textures
    out << writeStringNull(map.TERRAIN_SHADER_PATH);
    out << writeStringNull(map.BACKGROUND_PATH);
    out << writeStringNull(map.SKYCUBE_PATH);
    out << writeInt(1); // cubemap count
    out << writeStringNull(map.CUBEMAP_NAME);
    out << writeStringNull(map.CUBEMAP_PATH);
    out << writeFloat(map.LIGHTING_MULTIPLIER);
    out << writeVector3f(map.SUN_DIRECTION);
    out << writeVector3f(map.SUN_AMBIANCE_COLOR);
    out << writeVector3f(map.SUN_COLOR);
    out << writeVector3f(map.SHADOW_COLOR);
    out << writeVector4f(map.SPECULAR_COLOR);
    out << writeFloat(map.BLOOM);
    out << writeVector3f(map.FOG_COLOR);
    out << writeFloat(map.FOG_START);
    out << writeFloat(map.FOG_END);
    // water
    out << writeInt<uint8_t>(1); // map has water
    out << writeFloat(map.WATER_HEIGHT);
    out << writeFloat(map.WATER_DEEP_HEIGHT);
    out << writeFloat(map.WATER_ABYSS_HEIGHT);
    out << writeVector3f(map.WATER_SURFACE_COLOR);
    out << writeVector2f(map.WATER_COLOR_LERP);
    out << writeFloat(map.WATER_REFRACTION);
    out << writeFloat(map.WATER_FRESNEL_BIAS);
    out << writeFloat(map.WATER_FRESNEL_POWER);
    out << writeFloat(map.WATER_UNIT_REFLECTION);
    out << writeFloat(map.WATER_SKY_REFLECTION);
    out << writeFloat(map.WATER_SUN_SHININESS);
    out << writeFloat(map.WATER_SUN_STRENGH);
    out << writeVector3f(map.WATER_SUN_DIRECTION);
    out << writeVector3f(map.WATER_SUN_COLOR);
    out << writeFloat(map.WATER_SUN_REFLECTION);
    out << writeFloat(map.WATER_SUN_GLOW);
    out << writeStringNull(map.WATER_CUBEMAP_PATH);
    out << writeStringNull(map.WATER_RAMP_PATH);

    // waves
    for (int i = 0; i < map.WAVE_NORMAL_COUNT; i++) {
      out << writeFloat(map.WAVE_NORMAL_REPEATS[i]);
    }
    for (int i = 0; i < map.WAVE_NORMAL_COUNT; i++)
    {
      out << writeVector2f(map.WAVE_NORMAL_MOVEMENTS[i]);
      out << writeStringNull(map.WAVE_TEXTURE_PATHS[i]);
    }

    // wave generators
    out << writeInt(0); // wave generator count

    // terrain textures
    for (int i = 0; i < 24; i++) {
      out << writeInt<uint8_t>(0); // unknown
    }
    for (int i = 0; i < map.TERRAIN_TEXTURE_COUNT; ++i)
    {
      out << writeStringNull(map.TERRAIN_TEXTURE_PATHS[i]);
      out << writeFloat(map.TERRAIN_TEXTURE_SCALES[i]);
    }
    for (int i = 0; i < map.TERRAIN_NORMALS_COUNT; ++i)
    {
      out << writeStringNull(map.TERRAIN_NORMALS_PATHS[i]);
      out << writeFloat(map.TERRAIN_NORMALS_SCALES[i]);
    }

    out << writeInt(0); // unknown
    out << writeInt(0); // unknown

    // decals
    out << writeInt(0); // decal count
    out << writeInt(0); // decal group count
    out << writeInt(map.size); // width
    out << writeInt(map.size); // height

    // normal maps
    out << writeInt(1); // normal map count
    out << writeInt<int32_t>(sizeof(DDS_HEADER_2) + map.normalMap.total() * map.normalMap.elemSize()); // normalmap byte count
    out.write(DDS_HEADER_2, sizeof(DDS_HEADER_2)); // dds header
    out.write(reinterpret_cast<const char*>(map.normalMap.ptr()), map.normalMap.total() * map.normalMap.elemSize()); // normalmap data

    // texture maps
    out << writeInt<int32_t>(sizeof(DDS_HEADER_1) + map.textureMasksLow.total() * map.textureMasksLow.elemSize()); // texture masks low byte count
    out.write(DDS_HEADER_1, sizeof(DDS_HEADER_1)); // dds header

    out.write(reinterpret_cast<const char*>(map.textureMasksLow.ptr()), map.textureMasksLow.total() * map.textureMasksLow.elemSize());
    out << writeInt<int32_t>(sizeof(DDS_HEADER_1) + map.textureMasksHigh.total() * map.textureMasksHigh.elemSize()); // texture maks high byte count
    out.write(DDS_HEADER_1, sizeof(DDS_HEADER_1)); // dds header
    out.write(reinterpret_cast<const char*>(map.textureMasksHigh.ptr()), map.textureMasksHigh.total() * map.textureMasksHigh.elemSize()); // texture masks high data

    // water maps
    out << writeInt(1); // unknown

    out << writeInt<int32_t>(sizeof(DDS_HEADER_3) + map.waterMap.total() * map.waterMap.elemSize()); // tex watermap byte count
    out.write(DDS_HEADER_3, sizeof(DDS_HEADER_3)); // dds header
    out.write(reinterpret_cast<const char*>(map.waterMap.ptr()), map.waterMap.total() * map.waterMap.elemSize()); // watermap data
    out.write(reinterpret_cast<const char*>(map.waterFoamMask.ptr()), map.waterFoamMask.total() * map.waterFoamMask.elemSize()); // watermap data
    out.write(reinterpret_cast<const char*>(map.waterFlatnessMask.ptr()), map.waterFlatnessMask.total() * map.waterFlatnessMask.elemSize()); // watermap data
    out.write(reinterpret_cast<const char*>(map.waterDepthBiasMask.ptr()), map.waterDepthBiasMask.total() * map.waterDepthBiasMask.elemSize()); // watermap data

    // terrain type
    out.write(reinterpret_cast<const char*>(map.terrainType.ptr()), map.terrainType.total() * map.terrainType.elemSize()); // watermap data

    // props
    out << writeInt<int32_t>(map.props.size());
    for (int i = 0; i < map.props.size(); ++i)
    {
      out << writeStringNull(map.props[i].path);
      out << writeVector3f(map.props[i].position);
      out << writeVector3f({static_cast<float>(std::sin(map.props[i].rotation * 2 * M_PI)), 0.f, static_cast<float>(std::cos(map.props[i].rotation * 2 * M_PI))});
      out << writeVector3f({0.f, 1.f, 0.f});
      out << writeVector3f({static_cast<float>(-std::cos(map.props[i].rotation * 2 * M_PI)), 0.f, static_cast<float>(std::sin(map.props[i].rotation * 2 * M_PI))});
      out << writeVector3f({1,1,1}); //scale
    }
  }

  template<typename T>
  class writeInt
  {
  private:
    T i;

  public:
    writeInt(T i): i(i) {}
    friend std::ostream& operator << (std::ostream& os, writeInt const& b)
    {
      os.write(reinterpret_cast<const char *>(&(b.i)), sizeof(T));
      return os;
    }
  };

  class writeFloat
  {
  private:
    float a;

  public:
    writeFloat(float a): a(a) {}
    friend std::ostream& operator << (std::ostream& os, writeFloat const& b)
    {
      os.write(reinterpret_cast<const char *>(&(b.a)), sizeof(float));
      return os;
    }
  };

  class writeStringNull
  {
  private:
    std::string s;

  public:
    writeStringNull(std::string const& s): s(s) {}
    friend std::ostream& operator << (std::ostream& os, writeStringNull const& s)
    {
      os << s.s;
      os << writeInt<uint8_t>(0);
      return os;
    }
  };

  class writeVector2f
  {
  private:
    Eigen::Vector2f v;

  public:
    writeVector2f(Eigen::Vector2f const& v): v(v) {}
    friend std::ostream& operator << (std::ostream& os, writeVector2f const& v)
    {
      os << writeFloat(v.v.x()) << writeFloat(v.v.y());
      return os;
    }
  };

  class writeVector3f
  {
  private:
    Eigen::Vector3f v;

  public:
    writeVector3f(Eigen::Vector3f const& v): v(v) {}
    friend std::ostream& operator << (std::ostream& os, writeVector3f const& v)
    {
      os << writeFloat(v.v.x()) << writeFloat(v.v.y()) << writeFloat(v.v.z());
      return os;
    }
  };

  class writeVector4f
  {
  private:
    Eigen::Vector4f v;

  public:
    writeVector4f(Eigen::Vector4f const& v): v(v) {}
    friend std::ostream& operator << (std::ostream& os, writeVector4f const& v)
    {
      os << writeFloat(v.v.x()) << writeFloat(v.v.y()) << writeFloat(v.v.z()) << writeFloat(v.v.w());
      return os;
    }
  };

};

}
