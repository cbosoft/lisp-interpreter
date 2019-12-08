#include "types.hpp"


LispFileIO::LispFileIO(std::string path, std::ios_base::openmode mode) {
  this->fstr = std::fstream(path, mode);
}

void LispFileIO::write(std::string s)
{
  if (this->is_file)
    this->fstr << s;
}

void LispFileIO::writeline(std::string s)
{
  if (this->is_file)
    this->fstr << s << std::endl;
}

std::string LispFileIO::read()
{
  std::stringstream buffer;
  buffer << this->fstr.rdbuf();
  return buffer.str();
}

std::string LispFileIO::readline()
{
  std::string buffer;
  std::getline(this->fstr, buffer);
  return buffer;
}
