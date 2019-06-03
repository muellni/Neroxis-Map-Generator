#pragma once

#include "export/SCMapExporter.h"
#include "map/ConcurrentBinaryMask.h"
#include "generator/Random.h"

namespace faf {

static auto gen_SCMapExporter_1 = [](std::ostream& out)
{
  SCMap map(32, 3, 3, 3);
  map.props.push_back({"asdf", {3,4,5}, 3.7});
  SCMapExporter().exportSCMAP(out, map);
};

static auto gen_SCMapExporter_2 = [](std::ostream& out)
{

};

}
