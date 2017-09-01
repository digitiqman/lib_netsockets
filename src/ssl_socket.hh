#ifndef SSL_SOCKET_HH
#define SSL_SOCKET_HH 1

#include <string>

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
//ssl_socket_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class ssl_socket_t
{
public:
  ssl_socket_t();
  ~ssl_socket_t();
  int open(const char *str_ip);
  void close();
  int send(const char *buf);
  int receive();

  //buffer to store response
  std::string m_response;

protected:
  int parse_http_headers();
  SSL *m_ssl;
  int m_socket_ssl;
  int m_socket_fd;
};

#endif

