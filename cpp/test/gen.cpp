#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <functional>
#include <filesystem>

#include "test/gen_rand.h"
#include "test/gen_BinaryMask.h"

namespace faf {

void rungens()
{
  std::map<std::string, std::function<void (std::ostream&)>> gens;
  gens.insert(std::make_pair("gen_rand_int", gen_rand_int));
  gens.insert(std::make_pair("gen_rand_int_bound", gen_rand_int_bound));
  gens.insert(std::make_pair("gen_rand_float", gen_rand_float));
  gens.insert(std::make_pair("gen_BinaryMask_rand", gen_BinaryMask_rand));
  gens.insert(std::make_pair("gen_BinaryMask_invert", gen_BinaryMask_invert));
  gens.insert(std::make_pair("gen_BinaryMask_enlarge", gen_BinaryMask_enlarge));
  gens.insert(std::make_pair("gen_BinaryMask_shrink", gen_BinaryMask_shrink));
  gens.insert(std::make_pair("gen_BinaryMask_inflate", gen_BinaryMask_inflate));
  gens.insert(std::make_pair("gen_BinaryMask_deflate", gen_BinaryMask_deflate));
  gens.insert(std::make_pair("gen_BinaryMask_cutCorners", gen_BinaryMask_cutCorners));
  gens.insert(std::make_pair("gen_BinaryMask_acid", gen_BinaryMask_acid));
  gens.insert(std::make_pair("gen_BinaryMask_outline", gen_BinaryMask_outline));
  gens.insert(std::make_pair("gen_BinaryMask_smooth", gen_BinaryMask_smooth));
  gens.insert(std::make_pair("gen_BinaryMask_combine", gen_BinaryMask_combine));
  gens.insert(std::make_pair("gen_BinaryMask_intersect", gen_BinaryMask_intersect));
  gens.insert(std::make_pair("gen_BinaryMask_minus", gen_BinaryMask_minus));
  gens.insert(std::make_pair("gen_BinaryMask_fillCircle", gen_BinaryMask_fillCircle));
  gens.insert(std::make_pair("gen_BinaryMask_trimEdge", gen_BinaryMask_trimEdge));
  gens.insert(std::make_pair("gen_BinaryMask_getRandomPosition", gen_BinaryMask_getRandomPosition));

  std::filesystem::path gendir("/tmp");
  auto full_path = gendir / "cpp";
  std::filesystem::create_directories(full_path);

  for(decltype(gens)::const_iterator it = gens.begin(), end = gens.end(); it != end; ++it)
  {
    std::ofstream file;
    file.open((full_path/it->first).string() + ".gendata");
    it->second(file);
  }
}

}

int main(int argc, char *argv[])
{
  faf::rungens();
  return 0;
}
