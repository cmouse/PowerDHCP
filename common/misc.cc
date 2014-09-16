#include "powerdhcp.hh"

namespace PowerDHCP {
  Logger theL;

  pthread_rwlock_t Logger::d_lock = PTHREAD_RWLOCK_INITIALIZER;

  const char* Logger::LEVEL[] = {
      "DEBUG",
      "INFO",
      "NOTICE",
      "WARNING",
      "ERROR",
      "CRITICAL"
  };

  std::string stringerror() {
    std::string ret;
    char *buf = new char[1024];
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! defined(_GNU_SOURCE)
    if (strerror_r(errno, buf, 1024)==0)
      ret.assign(buf);
#elif defined(_GNU_SOURCE)
    if (strerror_r(errno, buf, 1024))
      ret.assign(buf);
#else
    #warning "PowerDHCP::stringerror might not be threadsafe on this system"
    ret.assign(strerror(errno)); 
#endif
    delete [] buf;
    return ret;
  }
};
