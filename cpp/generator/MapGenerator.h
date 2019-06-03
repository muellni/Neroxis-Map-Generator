#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>

#include "generator/Random.h"
#include "generator/MarkerGenerator.h"
#include "generator/PropGenerator.h"
#include "export/SaveExporter.h"
#include "export/ScenarioExporter.h"
#include "export/SCMapExporter.h"
#include "export/ScriptExporter.h"
#include "map/BinaryMask.h"
#include "map/FloatMask.h"
#include "map/ConcurrentBinaryMask.h"
#include "map/ConcurrentFloatMask.h"

namespace faf {

struct MapGenerator {

  inline static const std::string VERSION = "0.1.0";

  void save(std::filesystem::path const& folder, std::string const& mapName, SCMap const& map, long seed)
  {
    std::filesystem::remove_all(folder/mapName);
    std::filesystem::create_directories(folder/mapName);
    SCMapExporter::exportSCMAP(folder, mapName, map);
    SaveExporter::exportSave(folder, mapName, map);
    ScenarioExporter::exportScenario(folder, mapName, map);
    ScriptExporter::exportScript(folder, mapName, map);
  }

  SCMap generate(long seed)
  {
    auto startTime = std::chrono::system_clock::now();
    Random random(seed);
    SCMap map(512, 6, 64, 10);

    ConcurrentBinaryMask land(16, random.nextLong(), "land");
    ConcurrentBinaryMask mountains(32, random.nextLong(), "mountains");
    ConcurrentBinaryMask plateaus(32, random.nextLong(), "plateaus");
    ConcurrentBinaryMask ramps(128, random.nextLong(), "ramps");

    land.randomize(0.2f).inflate(1).cutCorners().enlarge(32).acid(0.5f).enlarge(128).smooth(4).acid(0.5f);
    mountains.randomize(0.05f).inflate(1).cutCorners().acid(0.5f).enlarge(128).smooth(4).acid(0.5f);
    plateaus.randomize(0.1f).inflate(1).cutCorners().acid(0.5f).enlarge(128).smooth(4).acid(0.5f);
    ramps.randomize(0.1f);

    plateaus.intersect(land).minus(mountains);
    ramps.intersect(plateaus).outline().minus(plateaus).intersect(land).minus(mountains).inflate(2);
    land.combine(mountains);

    land.enlarge(513).smooth(6);
    mountains.enlarge(513).inflate(1).smooth(6);
    plateaus.enlarge(513).inflate(1).smooth(6);

    ramps.enlarge(513).smooth(6);

    ConcurrentFloatMask heightmapBase(513, random.nextLong(), "heightmapBase");
    ConcurrentFloatMask heightmapLand(513, random.nextLong(), "heightmapLand");
    ConcurrentFloatMask heightmapMountains(513, random.nextLong(), "heightmapMountains");
    ConcurrentFloatMask heightmapPlateaus(513, random.nextLong(), "heightmapPlateaus");

    heightmapBase.init(land, 25.f, 25.f);
    heightmapLand.maskToHeightmap(0.025f, 0.25f, 95, land).smooth(2);

    heightmapMountains.maskToMoutains(2.f, 0.5f, mountains);
    plateaus.combine(mountains);
    heightmapPlateaus.init(plateaus, 0, 3.f).smooth(5.f, ramps);
    heightmapMountains.add(heightmapPlateaus).smooth(1);

    ConcurrentBinaryMask grass(land, random.nextLong(), "grass");
    ConcurrentFloatMask grassTexture(256, random.nextLong(), "grassTexture");
    ConcurrentBinaryMask rock(mountains, random.nextLong(), "rock");
    ConcurrentFloatMask rockTexture(256, random.nextLong(), "rockTexture");

    heightmapBase.add(heightmapLand);
    heightmapBase.add(heightmapMountains);

    grass.deflate(6.f).combine(plateaus).shrink(256).inflate(1);

    grassTexture.init(grass, 0, 0.999f).smooth(2);

    ConcurrentBinaryMask plateaus2(plateaus, random.nextLong(), "plateaus2");
    plateaus.outline().inflate(2).minus(ramps);
    plateaus2.deflate(1).outline().inflate(2).minus(ramps);
    rock.inflate(3).combine(plateaus).combine(plateaus2).shrink(256);
    rockTexture.init(rock, 0, 0.999f).smooth(1);

    grass.minus(rock);

    map.setHeightmap(heightmapBase);
    map.heightmap.at<uint16_t>(0, 0) = 0;

    std::cout << "Terrain generation done: ";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();
    std::cout << " ms" << std::endl;

    MarkerGenerator markerGenerator(map, random.nextLong());
    BinaryMask spawnsMask(grass, random.nextLong());
    spawnsMask.enlarge(513).minus(ramps).deflate(16).trimEdge(20).fillCircle(256, 256, 128, false);
    markerGenerator.generateSpawns(spawnsMask, 64);
    BinaryMask resourceMask(grass.minus(rock), random.nextLong());
    resourceMask.enlarge(513).minus(ramps).deflate(5);
    markerGenerator.generateMexs(resourceMask);
    markerGenerator.generateHydros(resourceMask);

    BinaryMask noProps(rock, random.nextLong());
    noProps.combine(ramps);
    for (int i = 0; i < map.spawns.size(); i++)
    {
      noProps.fillCircle(map.spawns[i].x(), map.spawns[i].z(), 30, true);
    }
    for (int i = 0; i < map.mexs.size(); i++)
    {
      noProps.fillCircle(map.mexs[i].x(), map.mexs[i].z(), 5, true);
    }
    for (int i = 0; i < map.hydros.size(); i++)
    {
      noProps.fillCircle(map.hydros[i].x(), map.hydros[i].z(), 7, true);
    }

    PropGenerator propGenerator(map, random.nextLong());
    BinaryMask treeMask(32, random.nextLong());
    treeMask.randomize(0.2f).inflate(1).cutCorners().acid(0.5f).enlarge(128).smooth(4).acid(0.5f);
    BinaryMask fieldStoneMask(treeMask, random.nextLong());
    treeMask.enlarge(256).intersect(grass);
    fieldStoneMask.invert().enlarge(256).intersect(grass);
    treeMask.enlarge(513).deflate(5).fillCircle(256, 256, 96, false).minus(noProps).trimEdge(3);
    fieldStoneMask.enlarge(513).deflate(5).fillCircle(256, 256, 96, true).minus(noProps).trimEdge(10);

    propGenerator.generateProps(treeMask, PropGenerator::TREE_GROUPS(), 3.f);
    propGenerator.generateProps(treeMask, PropGenerator::ROCKS(), 10.f);
    propGenerator.generateProps(fieldStoneMask, PropGenerator::FIELD_STONES(), 30.f);

    BinaryMask lightGrass(grass, random.nextLong());
    lightGrass.randomize(0.5f);
    lightGrass.minus(rock).intersect(grass).minus(treeMask.shrink(256));
    FloatMask lightGrassTexture(256, random.nextLong());
    lightGrassTexture.init(lightGrass, 0, 0.999f).smooth(2);

    map.setTextureMaskLow(grassTexture, lightGrassTexture, rockTexture, FloatMask(513, 0));

    land.shrink(256);

    for (int x = 0; x < 256; x++)
    {
      for (int y = 0; y < 256; y++)
      {
        unsigned char red = 0;
        unsigned char green = 0;
        unsigned char blue = 127;
        if (land(y, x))
        {
          red = 191;
          green = 191;
          blue = 0;
        }
        if (grass(y, x))
        {
          red = 0;
          green = 127;
          blue = 0;
        }
        if (lightGrass(y, x))
        {
          red = 0;
          green = 191;
          blue = 0;
        }
        if (rock(y, x))
        {
          red = 96;
          green = 96;
          blue = 96;
        }
        map.preview.at<cv::Vec4b>(x, y) = {red, green, blue, 255};
      }
    }

    return map;
  }
};

}
