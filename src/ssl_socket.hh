#ifndef SSL_SOCKET_HH
#define SSL_SOCKET_HH 1

//std
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <ctime>
#include <assert.h>
#include <stdint.h>
#include <sstream>
#include <time.h> 

#if defined (_MSC_VER)
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <openssl/ssl.h>
#include <openssl/err.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////
//time_value_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class time_value_t
{
public:
  size_t time;
  float value;
  std::string wave;
  time_value_t(size_t t, float v) :
    time(t),
    value(v)
  {
  }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class ssl_socket_t
{
public:
  ssl_socket_t();
  ~ssl_socket_t();
  int open(int period, int days, std::string ticker, const char *str_ip);
  void close();
  int send(const char *buf);
  int receive();
  int parse_http_headers();
  std::vector<time_value_t> m_tv;
protected:
  SSL *m_ssl;
  int m_socket_ssl;
  int m_socket_fd;
};

#endif

