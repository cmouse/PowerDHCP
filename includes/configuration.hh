namespace PowerDHCP {
  void initialize(int argc, const char **argv);

  class ConfigurationException: public Exception {
  public:
    ConfigurationException(const std::string& file, const std::string &reason, size_t lineno) THROW {
      this->reason = std::string("Error while reading ") + file + std::string(": at ") + boost::lexical_cast<std::string>(lineno) + std::string(": ") + reason;
    };
    ~ConfigurationException() THROW {};
  };

  class ConfigurationKeyNotFoundException: public Exception {
  public:
    ConfigurationKeyNotFoundException(const std::string& key) THROW {
      this->reason = std::string("Configuration key ") + key + std::string(" is unknown");
    };
    ~ConfigurationKeyNotFoundException() THROW {};
  };

  class ConfigValue {
  public:
    ConfigValue() { defined = false; null = true; };
    ~ConfigValue() {};
   
    template<typename T> const T as() const { if (!isDefined()) { return boost::lexical_cast<T>(defval); } else { return boost::lexical_cast<T>(value); } };
    bool isDefined() const { return defined; };
    bool isNull() const { return null; }; 
    template<typename T> void set(T value) { this->value = boost::lexical_cast<std::string>(value); defined = true; null = false; };
    void setNull() { this->value = ""; null = true; defined = true; };
    void load(const std::string& value) { this->value = value; defined = true; null = false; };
    void setDescription(const std::string& description) { this->descr = description; };
    const std::string& description() const { return this->descr; };
    const std::string& str() const { return this->value; };
    const std::string& getDefault() const { return this->defval; };
    template<typename T> void setDefault(const T& defval) { this->defval = boost::lexical_cast<std::string>(defval); };
    template<typename T> std::vector<T>& toArray(std::vector<T>& vec) const {
      // tokenize
      boost::char_separator<char> sep(",");
      boost::tokenizer<boost::char_separator<char> > tokenizer(value, sep);
      for(boost::tokenizer<boost::char_separator<char> >::iterator i = tokenizer.begin(); i != tokenizer.end(); i++) {
        vec.push_back(boost::lexical_cast<T>(*i));
      }
      return vec;
    };
    template<typename T> void fromArray(const std::vector<T>& vec) {
      std::ostringstream oss;
      BOOST_FOREACH(T val, vec) {
        oss << boost::lexical_cast<std::string>(val) << ", ";
      }
      value = "";
      defined = true;
      null = false;
      if (!oss.str().empty()) {
        value = oss.str().substr(0, oss.str().size() - 2);
      }
    };
  private:
    bool defined;
    bool null;

    std::string defval;
    std::string value;
    std::string descr;
  };

  class Configuration : boost::noncopyable {
  public:
    Configuration() {};
    ~Configuration();

    enum UnknownKeyAction { Ignore, Reject };

    void load(const std::string& file, UnknownKeyAction act);
    void load(int argc, const char** argv, UnknownKeyAction act);
    void clear() { values.clear(); };
    bool has(const std::string& key) const {
      return (values.count(key)>0);
    };
    ConfigValue& operator[](const std::string& key) {
      if (values.count(key)) return values[key];
      throw ConfigurationKeyNotFoundException(key);
    }
    ConfigValue& define(const std::string& key, const std::string& description) { values[key].setDescription(description); return values[key]; };
    template<typename T> ConfigValue& define(const std::string& key, const std::string& description, const T& defval) { values[key].setDescription(description); values[key].setDefault<T>(defval); return values[key]; };

    friend std::ostream& operator<< (std::ostream& stream, const Configuration& configuration);

    void swap(Configuration& s) { std::swap(s.values, values); };

  private:
    std::map<std::string, ConfigValue> values;
    std::vector<std::string> includes;

    void parseLine(const std::string& line, const std::string& file, size_t& lineno, std::string& sect, UnknownKeyAction act);
  };

  extern Configuration config;
};
