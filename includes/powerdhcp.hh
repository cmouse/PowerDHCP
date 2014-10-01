#pragma once
#include "config.h"
#include <iostream>
#include <cstdlib>
#include <limits.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <list>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#include <strings.h>
#include <algorithm>

#ifdef HAVE_CXX11
#define THROW noexcept
#else
#define THROW throw()
#endif

namespace PowerDHCP {
  class Exception: public std::exception {
  public:
    Exception() THROW {};
    Exception(const std::string& reason) THROW { this->reason = reason; };
    Exception& operator= (const PowerDHCP::Exception& ce) THROW { this->reason = ce.reason; return *this; };
    ~Exception() THROW {};
    virtual const char* what() const THROW { return reason.c_str(); };

    std::string reason;
  };
};

#include "definitions.hh"
#include "lock.hh"
#include "configuration.hh"
#include "logging.hh"
#include "packet.hh"
