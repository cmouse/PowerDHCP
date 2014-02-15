#include "powerdhcp.hh"

namespace PowerDHCP {
  void initialize(int argc, const char **argv) {
    // setup defaults
    config.define<std::string>("module-dir", "Directory containing modules",LIBDIR "/powerdhcp");
    config.define<std::string>("config-dir", "Directory containing configuration file", "/powerdhcp");
    config.define<std::string>("config-file", "Configuration file name", SYSCONFDIR "/dhcpd.conf");
    config.define("modules", "Comma separated list of modules to load");
   
    // load configuration data first pass
    config.load(argc, argv, Configuration::Ignore);
    std::string path = config["config-dir"].str();
    path.append("/").append(config["config-file"].str());
    config.load(path, Configuration::Ignore);

    // setup defaults
    config.define<bool>("unicast-only", "Only listen for unicast requests", false);
    
    // launch modules
    // parse everything again
    config.clear();
    config.load(path, Configuration::Reject);
    config.load(argc, argv, Configuration::Reject);
  };
};
