#pragma once

#include <fstream>
#include <filesystem>

#include "map/SCMap.h"

namespace faf {

struct SaveExporter
{
  static void exportSave(std::filesystem::path const& folderPath, std::string mapname, SCMap const& map)
  {
    std::ofstream out;
    out.open((folderPath / mapname / (mapname + "_save.lua")).string());
    out << "Scenario = {\n";
    out << "  next_area_id = '1',\n";
    out << "  Props = {},\n";
    out << "  Areas = {},\n";
    out << "  MasterChain = {\n";
    out << "    ['_MASTERCHAIN_'] = {\n";
    out << "      Markers = {\n";
    for(int i = 0; i < map.spawns.size(); ++i)
    {
      out << "        ['ARMY_" << (i+1) << "'] = {\n";
      out << "          ['type'] = STRING( 'Blank Marker' ),\n";
      out << "          ['position'] = VECTOR3( " << (map.spawns[i].x() + 0.5f) << ", " << map.spawns[i].y() << ", " << (map.spawns[i].z() + 0.5f) << " ),\n";
      out << "          ['orientation'] = VECTOR3( 0.00, 0.00, 0.00 ),\n";
      out << "          ['color'] = STRING( 'ff800080' ),\n";
      out << "          ['prop'] = STRING( '/env/common/props/markers/M_Blank_prop.bp' ),\n";
      out << "        },\n";
    }
    for(int i = 0; i < map.mexs.size(); ++i)
    {
      out << "        ['MASS_" << (i+1) << "'] = {\n";
      out << "          ['size'] = FLOAT( 1.000000 ),\n";
      out << "          ['resource'] = BOOLEAN( true ),\n";
      out << "          ['amount'] = FLOAT( 100.000000 ),\n";
      out << "          ['color'] = STRING( 'ff808080' ),\n";
      out << "          ['type'] = STRING( 'Mass' ),\n";
      out << "          ['prop'] = STRING( '/env/common/props/markers/M_Mass_prop.bp' ),\n";
      out << "          ['orientation'] = VECTOR3( 0, 0, 0 ),\n";
      out << "          ['position'] = VECTOR3( " << (map.mexs[i].x() + 0.5f) << ", " << map.mexs[i].y() << ", " << (map.mexs[i].z() + 0.5f) << " ),\n";
      out << "        },\n";
    }
    for(int i = 0; i < map.hydros.size(); ++i)
    {
      out << "        ['Hydrocarbon_" << (i+1) <<"'] = {\n";
      out << "          ['size'] = FLOAT( 3.00 ),\n";
      out << "          ['resource'] = BOOLEAN( true ),\n";
      out << "          ['amount'] = FLOAT( 100.000000 ),\n";
      out << "          ['color'] = STRING( 'ff808080' ),\n";
      out << "          ['type'] = STRING( 'Hydrocarbon' ),\n";
      out << "          ['prop'] = STRING( '/env/common/props/markers/M_Hydrocarbon_prop.bp' ),\n";
      out << "          ['orientation'] = VECTOR3( 0, 0, 0 ),\n";
      out << "          ['position'] = VECTOR3( " << (map.hydros[i].x() + 0.5f) << ", " << map.hydros[i].y() << ", " << (map.hydros[i].z() + 0.5f) << " ),\n";
      out << "        },\n";
    }
    out << "      },\n";
    out << "    },\n";
    out << "  },\n";
    out << "  Chains = {},\n";
    out << "  next_queue_id = '1',\n";
    out << "  Orders = {},\n";
    out << "  next_platoon_id = '1',\n";
    out << "  Platoons = {},\n";
    out << "  next_army_id = '1',\n";
    out << "  next_group_id = '1',\n";
    out << "  next_unit_id = '1',\n";
    out << "  Armies = {\n";
    for(int i = 0; i < map.spawns.size(); ++i)
    {
      saveArmy("ARMY_" + std::to_string(i+1), out);
    }
    saveArmy("ARMY_9", out);
    saveArmy("NEUTRAL_CIVILIAN", out);
    out << "  },\n";
    out << "}\n";
  }

protected:
  static void saveArmy(std::string const& name, std::ostream& out)
  {
    out << "    ['" << name << "'] = {\n";
    out << "      personality = '',\n";
    out << "      plans = '',\n";
    out << "      color = 0,\n";
    out << "      faction = 0,\n";
    out << "      Economy = {mass = 0, energy = 0},\n";
    out << "      Alliances = {},\n";
    out << "      ['Units'] = GROUP {\n";
    out << "        orders = '',\n";
    out << "        platoon = '',\n";
    out << "        Units = {\n";
    out << "          ['INITIAL'] = GROUP {\n";
    out << "            orders = '',\n";
    out << "            platoon = '',\n";
    out << "            Units = {},\n";
    out << "          },\n";
    out << "        },\n";
    out << "      },\n";
    out << "      PlatoonBuilders = {\n";
    out << "        next_platoon_builder_id = '0',\n";
    out << "        Builders = {},\n";
    out << "      },\n";
    out << "    },\n";
  }
};


}
