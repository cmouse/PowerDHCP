#include <syslog.h>

namespace PowerDHCP {
  class Logger : boost::noncopyable {
  public:
    enum LogLevel {
      DEBUG    = 0,
      INFO     = 1,
      NOTICE   = 2,
      WARNING  = 3,
      ERROR    = 4,
      CRITICAL = 5
    };

    static const char* LEVEL[]; 

    class LogObject : boost::noncopyable {
    public:
      LogObject(Logger *logger) { this->logger = logger; };
      LogObject& operator<<(LogLevel l) { this->level = l; return *this; };
      LogObject& operator<<(const std::string& s) { str << s; return *this; };
      LogObject& operator<<(int i) { str << i; return *this; };
      LogObject& operator<<(double i) { str << i; return *this; };
      LogObject& operator<<(long i) { str << i; return *this; };
      ~LogObject() {
        logger->log(level, str.str());
      }
    private:
      Logger *logger;
      std::ostringstream str;
      LogLevel level;
    };

    Logger() { reinitialize(); };
    ~Logger() { 
      WriteLock wl(&d_lock);
      if (do_syslog) closelog(); 
      if (do_logfile) d_ofs_file.close();
    };

    void reinitialize() {
      bool log_file_failed = false;
      {
        WriteLock wl(&d_lock);
        if (do_syslog) closelog();
        if (do_logfile) d_ofs_file.close();

        do_syslog  = config["log.syslog"].as<bool>();
        do_logfile = config["log.file"].str().empty() == false;
        d_logfile  = config["log.file"].str();
        d_loglevel = config["log.level"].as<int>();
        do_console = config["log.stderr"].as<bool>();
        d_ident    = config["log.ident"].str();
        d_facility = facility2int(boost::to_lower_copy(config["log.facility"].str()));

        if (do_syslog) openlog(d_ident.c_str(), d_facility, LOG_NDELAY|LOG_PID);
        try {
          if (do_logfile) d_ofs_file.open(d_logfile.c_str(), std::ofstream::out | std::ofstream::app);
        } catch (...) {
          log_file_failed = true;
          do_logfile = false; 
        }
      }

      if (log_file_failed)
        log(ERROR, "Could not open log file " + d_logfile + " for writing");
    }
  
    LogObject log() {
      return LogObject(this);
    }

    void log(LogLevel level, const std::string& str) {
      WriteLock wl(&d_lock);
      char tbuf[128];
      struct tm tm;
      time_t t;
      t = time((time_t*)NULL);
      localtime_r(&t, &tm);
      strftime(tbuf, sizeof tbuf, "[%Y-%m-%d %H:%M:%S] ", &tm);

      if (do_console) 
        std::cerr << tbuf << LEVEL[level] << ": " << str << std::endl;
      if (do_syslog) 
        syslog(7 - level, "%s", str.c_str());
      if (do_logfile) {
        if (d_ofs_file.is_open() && d_ofs_file.good())
          d_ofs_file << tbuf << LEVEL[level] << ": " << str << std::endl;
        else { // the file was already broken
          d_ofs_file.close();
          do_logfile = false;
          if (do_console)
            std::cerr << tbuf << LEVEL[4] << ": " << "Cannot write to log file " << d_logfile << std::endl;
          if (do_syslog) {
            syslog(LOG_ERR, "%s %s", "Cannot write to log file ", d_logfile.c_str());
          }
          return;
        }
        if (!d_ofs_file.good()) { // error occured after writing
          d_ofs_file.close();
          do_logfile = false;
          if (do_console)
            std::cerr << tbuf << LEVEL[4] << ": " << "Cannot write to log file " << d_logfile << std::endl;
          if (do_syslog) {
            syslog(LOG_ERR, "%s %s", "Cannot write to log file ", d_logfile.c_str());
          }
        }
      }
    }

  private:
    int facility2int(const std::string& facility) {
      try { 
        return boost::lexical_cast<int>(facility);
      } catch (...) {}  // mabe it's a string name?      
      if (facility == "auth") return LOG_AUTH;
      if (facility == "authpriv") return LOG_AUTHPRIV;
      if (facility == "cron") return LOG_CRON;
      if (facility == "daemon") return LOG_DAEMON;
      if (facility == "ftp") return LOG_FTP;
      if (facility == "kern") return LOG_KERN;
      if (facility == "local0") return LOG_LOCAL0;
      if (facility == "local1") return LOG_LOCAL1;
      if (facility == "local2") return LOG_LOCAL2;
      if (facility == "local3") return LOG_LOCAL3;
      if (facility == "local4") return LOG_LOCAL4;
      if (facility == "local5") return LOG_LOCAL5;
      if (facility == "local6") return LOG_LOCAL6;
      if (facility == "local7") return LOG_LOCAL7;
      if (facility == "lpr") return LOG_LPR;
      if (facility == "mail") return LOG_MAIL;
      if (facility == "news") return LOG_NEWS;
      if (facility == "syslog") return LOG_SYSLOG;
      if (facility == "user") return LOG_USER;
      if (facility == "uucp") return LOG_UUCP;
      throw PowerDHCP::Exception("Invalid syslog facility " + facility + " specified");
    }

    bool do_syslog;
    bool do_logfile;
    bool do_console;
    bool do_init;
    std::string d_logfile;
    int d_loglevel;
    std::string d_ident;
    int d_facility;
    std::ofstream d_ofs_file;

    static pthread_rwlock_t d_lock;
  };

  extern Logger theL;
};
