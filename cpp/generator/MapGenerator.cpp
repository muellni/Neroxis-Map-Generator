#include <iostream>
#include <string>

#include "generator/MapGenerator.h"

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    std::cerr << "Usage: generator [targetFolder] [seed] [expectedVersion] (mapName)" << std::endl;
    return 1;
  }

  std::string folderPath(argv[1]);
  long seed = std::atol(argv[2]);
  std::string version(argv[3]);
  std::string mapName(std::string("NeroxisGen_") + faf::MapGenerator::VERSION + "_" + std::to_string(seed));
  if (argc >= 5)
  {
    mapName = std::string(argv[4]);
  }
  if (version == faf::MapGenerator::VERSION)
  {
    faf::MapGenerator generator;
    std::cout << "Generating map " << mapName << std::endl;
    auto map = generator.generate(seed);
    std::cout << "Saving map to " << folderPath<< std::endl;
    generator.save(folderPath, mapName, map, seed);
    std::cout << "Done" << std::endl;
  }
  else
  {
    std::cerr << "This generator only supports version " << faf::MapGenerator::VERSION << std::endl;
    return 2;
  }

  return 0;
}
