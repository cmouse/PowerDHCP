#pragma once
#include "powerdhcp.hh"

namespace PowerDHCP {
  class DHCP4Option {
  public:
    int option;
    std::string vendor; // vendor value
    std::string value; // raw value

    virtual std::string str() const; // printable representation
  };

  class DHCP4Packet {
  public:
    DHCP4Packet() {};
    
    // getters
    int op() const { return d_op; };
    int htype() const { return d_htype; };
    int hlen() const { return d_hlen; };
    int hops() const { return d_hops; };
    int xid() const { return d_xid; };
    int secs() const { return d_secs; };
    int flags() const { return d_flags; };
    int ciaddr() const { return d_ciaddr; };
    int yiaddr() const { return d_yiaddr; };
    int siaddr() const { return d_siaddr; };
    const char* chaddr() const { return d_chaddr; };
    const std::string& sname() const { return d_sname; };
    const std::string& bootfile() const { return d_bootfile; };
    const std::vector<DHCP4Option>& options() const { return d_options; };
    const DHCP4Option& getOption(int optid, const std::string& vendor) const;

    // setters
    DHCP4Packet& op(int new_op) { this->d_op = new_op; return *this; };
    DHCP4Packet& htype(int new_htype) { this->d_htype = new_htype; return *this; };
    DHCP4Packet& hlen(int new_hlen) { this->d_hlen = new_hlen; return *this; };
    DHCP4Packet& hops(int new_hops) { this->d_hops = new_hops; return *this; };
    DHCP4Packet& xid(int new_xid) { this->d_xid = new_xid; return *this; };
    DHCP4Packet& secs(int new_secs) { this->d_secs = new_secs; return *this; };
    DHCP4Packet& flags(int new_flags) { this->d_flags = new_flags; return *this; };
    DHCP4Packet& ciaddr(int new_ciaddr) { this->d_ciaddr = new_ciaddr; return *this; };
    DHCP4Packet& yiaddr(int new_yiaddr) { this->d_yiaddr = new_yiaddr; return *this; };
    DHCP4Packet& siaddr(int new_siaddr) { this->d_siaddr = new_siaddr; return *this; };
    DHCP4Packet& chaddr(const char* new_chaddr, size_t chlen) { size_t len = std::min(chlen,static_cast<size_t>(16)); memcpy(d_chaddr, new_chaddr, len); return *this; };
    DHCP4Packet& sname(const std::string &new_sname) { this->d_sname = new_sname; return *this; };
    DHCP4Packet& bootfile(const std::string &new_bootfile) { this->d_bootfile = new_bootfile; return *this; };
    DHCP4Packet& options(const std::vector<DHCP4Option> &new_options) { this->d_options = new_options; return *this; };
    DHCP4Packet& setOption(int optid, std::string& optval, std::string& vendor);

  private:
    int d_op;
    int d_htype;
    int d_hlen;
    int d_hops;
    int d_xid;
    int d_secs;
    int d_flags;
    int d_ciaddr;
    int d_yiaddr;
    int d_siaddr;

    char d_chaddr[16]; 
    std::string d_sname;
    std::string d_bootfile;
    std::vector<DHCP4Option> d_options;
  };
}
