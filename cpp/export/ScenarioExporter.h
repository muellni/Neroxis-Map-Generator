#pragma once

#include <fstream>
#include <filesystem>

#include "map/SCMap.h"

namespace faf {

struct ScenarioExporter
{
  static void exportScenario(std::filesystem::path const& folderPath, std::string const& mapname, SCMap const& map)
  {
    std::ofstream out;
    out.open((folderPath / mapname / (mapname + "_scenario.lua")).string());
    out << "version = 3\n";
    out << "ScenarioInfo = {\n";
    out << "  name = '" + mapname + "',\n";
    out << "  description = '<LOC Empty_Description>',\n";
    out << "  type = 'skirmish',\n";
    out << "  starts = true,\n";
    out << "  preview = '',\n";
    out << "  size = {" << map.size << ", " << map.size << "},\n";
    out << "  map = '/maps/" << mapname << "/" << mapname << ".scmap',\n";
    out << "  map_version = 1,\n";
    out << "  save = '/maps/" << mapname + "/" << mapname << "_save.lua',\n";
    out << "  script = '/maps/" << mapname + "/" << mapname << "_script.lua',\n";
    out << "  norushradius = 50,\n";
    out << "  Configurations = {\n";
    out << "    ['standard'] = {\n";
    out << "      teams = {\n";
    out << "        {\n";
    out << "          name = 'FFA',\n";
    out << "          armies = {";
    for (int i = 0; i < map.spawns.size(); ++i)
    {
      out << "'ARMY_" << (i + 1) << "'";
      if(i < map.spawns.size()-1)
        out << ",";
    }
    out << "},\n";
    out << "        },\n";
    out << "      },\n";
    out << "    },\n";
    out << "  },\n";
    for (int i = 0; i < map.spawns.size(); ++i)
    {
      out << "  norushoffsetX_ARMY_" << (i + 1) << " = 0,\n";
      out << "  norushoffsetY_ARMY_" << (i + 1) << " = 0,\n";
    }
    out << "}\n";
  }
};

}
