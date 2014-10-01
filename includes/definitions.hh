#pragma once

namespace PowerDHCP {
  typedef std::map<std::string, std::string> strstrmap_t;
  std::string stringerror(); 

  extern void showHelp(const std::string& progName);
};
