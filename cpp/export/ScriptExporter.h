#pragma once

#include <fstream>
#include <filesystem>

#include "map/SCMap.h"

namespace faf {

struct ScriptExporter
{
  static void exportScript(std::filesystem::path const& folderPath, std::string const& mapname, SCMap const& map)
  {
    std::ofstream out;
    out.open((folderPath / mapname / (mapname + "_script.lua")).string());

    out << "local ScenarioUtils = import('/lua/sim/ScenarioUtilities.lua')\n";
    out << "function OnPopulate()\n";
    out << "ScenarioUtils.InitializeArmies()\n";
    out << "end\n";
    out << "function OnStart(self)\n";
    out << "end";
  }
};

}
