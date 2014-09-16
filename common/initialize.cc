#include "powerdhcp.hh"

namespace PowerDHCP {
  void initialize(int argc, const char **argv) {
    std::string path;

    // setup defaults
    config.define<std::string>("modules.dir", "Directory containing modules", PKGLIBDIR);
    config.define<std::string>("config.dir", "Directory containing configuration file", SYSCONFDIR "/powerdhcp");
    config.define<std::string>("config.file", "Configuration file name", "dhcpd.conf");
    config.define<bool>("config.load", "Whether to load config file", true);
    config.define<bool>("log.syslog", "Log to syslog", false);
    config.define<std::string>("log.file", "Log to file", "");
    config.define<bool>("log.stderr", "Log to stderr", false);
    config.define<int>("log.level", "Loglevel (0=debug,1=info,2=notice,3=warning,4=error,5=critical)", 1);
    config.define<std::string>("log.ident", "syslog identity to use", "dhcpd");
    config.define<std::string>("log.facility", "syslog facility to use (name or number)", "daemon");
    config.define("modules.load", "Comma separated list of modules to load");
    config.define<bool>("help", "show help", false);
    config.define<bool>("config", "dump configuration file", false);

    theL.reinitialize();   

    // load configuration data first pass
    config.load(argc, argv, Configuration::Ignore);
    if (config["config.load"].as<bool>()) {
      path = config["config.dir"].str();
      path.append("/").append(config["config.file"].str());
      config.load(path, Configuration::Ignore);
    }

    theL.reinitialize(); // this should be enough

    // setup defaults
    config.define<bool>("listen.unicast-only", "Only listen for unicast requests", false);
    config.define<bool>("listen.broadcast-only", "Only listen for broadcast requests", false);
    config.define<std::string>("listen.interfaces", "List of interfaces to listen (use all for all)", "all");
    config.define<std::string>("listen.ipv4_addresses", "List of IPv4 addresses to listen", "0.0.0.0");
    config.define<std::string>("listen.ipv6_addresses", "List of IPv6 addresses to listen", "::");
    config.define<std::string>("listen.ports", "List of UDP ports to listen", "67");

    // launch modules
    // parse everything again
    config.clear();
    if (config["config.load"].as<bool>()) {
      config.load(path, Configuration::Reject);
    }
    config.load(argc, argv, Configuration::Reject);

    if (config["help"].as<bool>()) {
      showHelp(argv[0]);
      exit(1);
    }
 
    if (config["config"].as<bool>()) {
      std::cerr << config << std::endl;
      exit(1);
    }
  };
};
