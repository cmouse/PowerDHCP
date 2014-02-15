#include "powerdhcp.hh"

int main(int argc, const char *argv[]) {
  try {
    PowerDHCP::initialize(argc, argv);
  } catch (PowerDHCP::Exception &pde) {
    std::cerr << "Could not initialize: " << pde.reason << std::endl;
  }

  return 0; 
};
