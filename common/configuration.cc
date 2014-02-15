#include "powerdhcp.hh"

#include <glob.h>

namespace PowerDHCP {

  Configuration config;

  Configuration::~Configuration() {};

  void Configuration::load(const std::string& file, UnknownKeyAction act) {
    size_t lineno=0;
    std::string sect;
    std::ifstream ifs(file.c_str());
  
    if (!ifs.is_open()) 
      throw ConfigurationException(file, "could not open", lineno);
  
    // load config file
    while(ifs.good()) {
      lineno++;
      std::string line;
      std::getline(ifs, line);
      if (!line.empty()) {
        boost::trim(line);
        parseLine(line, file, lineno, sect, act);
      }
    }
  
    ifs.close();
  
    // handle all includes
    BOOST_FOREACH(std::string path, includes) {
      glob_t globbuf;
      if (glob(path.c_str(), GLOB_TILDE_CHECK|GLOB_ERR, NULL, &globbuf)) 
        throw ConfigurationException(path, "cannot match include", lineno);
      for(size_t i = 0; i < globbuf.gl_pathc; i++) 
        load(globbuf.gl_pathv[i], act);
      globfree(&globbuf);
    }

    includes.clear(); // they be loaded.
  }
 
  void Configuration::parseLine(const std::string& line, const std::string& file, size_t& lineno, std::string& sect, UnknownKeyAction act) {
    std::string key;
    std::string value;
    bool add = false;
    std::string::size_type pos1,pos2;

    if (line[0] == ';') return; // comment line
    // check for section
    pos1 = line.find("[");
    if (pos1 != std::string::npos) {
      pos2 = line.find("]", pos1+1);
      if (pos2 == std::string::npos)
        throw ConfigurationException(file, "malformed line a", lineno);
      sect.assign(line.begin() + pos1 + 1, line.begin() + pos2);
      return;
    }
    pos1 = line.find_first_of("+=");
    if (pos1 == std::string::npos) {
      value = "1";
      // boolean. 
      key = boost::trim_copy(line);
      if (key.compare(0,3,"no-")==0) {
        // "not"
        key = key.substr(4);
        value = "0";
      }
    } else { // an actual value
      if (line[pos1] == '+') {
        // ensure next is =
        pos2 = pos1+1; // skip both +=
        if (pos2 >= line.size() || line[pos2] != '=')
          throw ConfigurationException(file, "malformed line b", lineno);
        add = true;
      } else pos2 = pos1; // just skip = 
  
      key = line.substr(0, pos1);
      boost::trim(key);
      if (sect.empty() == false) key = sect + std::string(".") + key;
  
      value = line.substr(pos2+1);
      boost::trim(value);
    }

    boost::to_lower(key);
    if (!sect.empty())
      key.insert(0, ".").insert(0, sect);

    if (key == "include") {
      includes.push_back(value);
      return;
    }

    if (values.count(key) == 0) {
      if (act == Ignore) return;
      throw ConfigurationKeyNotFoundException(key);
    }

    // then add it
    ConfigValue& cv = values[key];
    if (add) {
      if (!cv.isDefined())
        cv.load(value);
      else {
        std::ostringstream oss;
        oss << cv.as<std::string>() << ", " << value;
        cv.load(oss.str());
      }
    } else {
      cv.load(value);
    }
  }

  void Configuration::load(int argc, const char **argv, UnknownKeyAction act) {
    std::string sect;
    size_t lineno;

    for(int i=1;i<argc;i++) {
      lineno = i;
      std::string line(argv[i]);
      if (line.size() < 2 || line.compare(0,2,"--"))
        throw ConfigurationException("<stdin>", "malformed option", lineno);
      line.erase(0, 2);
      boost::trim(line);
      parseLine(line, std::string("<stdin>"), lineno, sect, act);
    }

    // handle all includes
    BOOST_FOREACH(std::string path, includes) {
      glob_t globbuf;
      if (glob(path.c_str(), GLOB_TILDE_CHECK|GLOB_ERR, NULL, &globbuf))
        throw ConfigurationException(path, "cannot match include", lineno);
      for(size_t i = 0; i < globbuf.gl_pathc; i++)
        load(globbuf.gl_pathv[i], act);
      globfree(&globbuf);
    }

    includes.clear(); // they be loaded.
  }

  std::ostream& operator<< (std::ostream& stream, const Configuration& configuration) {
    std::string sect;
    for(std::map<std::string, ConfigValue>::const_iterator i = configuration.values.begin(); i != configuration.values.end(); i++) {
      std::string key = i->first;
      std::string::size_type pos = key.find_last_of(".");
      if (pos != std::string::npos) {
        if (sect.size() != pos || sect.compare(pos, sect.size(), key)) {
          // section has changed
          sect = key.substr(0, pos);
          stream << std::endl << "[" << sect << "]" << std::endl;
        }
        key = key.substr(pos+1);
      }
      stream << "; " << i->second.description() << std::endl;
      if (i->second.isDefined())
        stream << key << " = " << i->second.str();
      else
        stream << ";" << key << " = " << i->second.getDefault();
      stream << std::endl;
    }
    return stream;
  }
};
