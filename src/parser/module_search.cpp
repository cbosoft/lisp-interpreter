#include <string>
#include <sstream>

#include <glob.h>

#include "../types.hpp"
#include "../util/exception.hpp"
#include "../util/formatter.hpp"
#include "parser.hpp"


// Looks around for the file with name $name, no extension
// returns full path to result
std::string LispParser::search_module(std::string name)
{
  std::stringstream ss;
  std::string possible_path;
  for (const auto search_path : this->module_paths) {
    glob_t glob_result;
    
    ss << search_path << name << ".cl";
    possible_path = ss.str();
    ss.str("");

    glob(possible_path.c_str(), 0, NULL, &glob_result);

    if (glob_result.gl_pathc >= 2) 
      throw ImportError(Formatter() << "Ambiguous result while searching for \"" << name << "\".");

    if (glob_result.gl_pathc == 1)
      return std::string(glob_result.gl_pathv[0]);
  }

  throw ImportError(Formatter() << "No module found with name \"" << name << "\"");
}
