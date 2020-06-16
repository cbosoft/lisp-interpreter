#include "types.hpp"

const std::string Traceable::repr_source()
{
  if (this->source == NULL)
    return "";

  std::stringstream rv;

  switch (this->source->type) {

    case TRACESOURCE_REPL:
      rv 
        << "from repl:" << std::endl 
        << "  " << this->source->path_or_commands;
      break;

    case TRACESOURCE_ARGUMENT:
      rv 
        << "from args:" << std::endl 
        << "  " << this->source->path_or_commands;
      break;

    case TRACESOURCE_FILE:
      rv 
        << "from file (\"" + this->source->path_or_commands + "\"):" << std::endl 
        << this->get_file_trace();
      break;
  }

  rv << std::endl;
  for (int i = -1; i < this->source->column-this->source->token_length; i++)
    rv << " ";
  for (int i = 0; i < this->source->token_length; i++)
    rv << "^";

  return rv.str();

}





const std::string Traceable::get_file_trace() const
{
  std::ifstream istr(this->source->path_or_commands);
  if (istr.fail())
    throw IOError("Error opening source file \"" + this->source->path_or_commands + "\".");

  std::string line;
  for (int i = 0; i < (this->source->row - 1); i++) {
    // skip down in file until on correct line
    if (!std::getline(istr, line))
      throw IOError("file ended unexpectedly.");
  }

  std::stringstream rv;
  for (int i = 0; i < 3; i++) {
    if (!std::getline(istr, line))
      break;
    rv << "  " << line;
    if (i > 0) rv << std::endl;
  }
  return rv.str();
}
