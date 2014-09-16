#include "powerdhcp.hh"

using namespace PowerDHCP;

int main(int argc, const char *argv[]) {
  try {
    initialize(argc, argv);
  } catch (PowerDHCP::Exception &pde) {
    std::cerr << "Could not initialize: " << pde.reason << std::endl;
  }

  L<<Logger::INFO<<"Hello, world";

  return 0; 
};

void PowerDHCP::showHelp(const std::string& progName) {
  std::cerr << "Usage: " << progName << " [options]" << std::endl;
  std::cerr << std::endl; 
  std::cerr << "    " << "You can use all configuration file options as parameters by prefixing them with --" << std::endl;
}
