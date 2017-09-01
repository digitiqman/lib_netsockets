#include "ssl_socket.hh"
#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::ssl_socket_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

ssl_socket_t::ssl_socket_t() :
  m_ssl(NULL)
{
#if defined (_MSC_VER)
  WSADATA ws_data;
  if (WSAStartup(MAKEWORD(2, 0), &ws_data) != 0)
  {
    exit(1);
  }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::~ssl_socket_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

ssl_socket_t::~ssl_socket_t()
{
#if defined (_MSC_VER)
  WSACleanup();
#endif
  ERR_free_strings();
  EVP_cleanup();
  if (m_ssl)
  {
    SSL_shutdown(m_ssl);
    SSL_free(m_ssl);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::close_socket()
/////////////////////////////////////////////////////////////////////////////////////////////////////

void ssl_socket_t::close_socket()
{
#if defined (_MSC_VER)
  closesocket(m_socket_fd);
#else
  close(m_socket_fd);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::open
/////////////////////////////////////////////////////////////////////////////////////////////////////

int ssl_socket_t::open(const char *str_ip)
{
  struct sockaddr_in server_addr;

  if ((m_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    return -1;
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(str_ip);
  server_addr.sin_port = htons(443);
  int socklen = sizeof(server_addr);

  if (connect(m_socket_fd, (struct sockaddr *)&server_addr, socklen))
  {
    return -1;
  }

  SSL_library_init();
  SSLeay_add_ssl_algorithms();
  SSL_load_error_strings();
  const SSL_METHOD *meth = TLSv1_2_client_method();
  SSL_CTX *ctx = SSL_CTX_new(meth);
  m_ssl = SSL_new(ctx);
  if (!m_ssl)
  {
    return -1;
  }

  m_socket_ssl = SSL_get_fd(m_ssl);
  SSL_set_fd(m_ssl, m_socket_fd);
  int err = SSL_connect(m_ssl);
  if (err <= 0)
  {
    return -1;
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::send
/////////////////////////////////////////////////////////////////////////////////////////////////////

int ssl_socket_t::send(const char *buf)
{
  std::cout << buf;
  int len = SSL_write(m_ssl, buf, strlen(buf));
  if (len < 0)
  {
    int err = SSL_get_error(m_ssl, len);
    switch (err)
    {
    case SSL_ERROR_WANT_WRITE:
      return 0;
    case SSL_ERROR_WANT_READ:
      return 0;
    case SSL_ERROR_ZERO_RETURN:
    case SSL_ERROR_SYSCALL:
    case SSL_ERROR_SSL:
    default:
      return -1;
    }
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::parse_http_headers
//Peeks at an incoming message.The data is treated as unread and the next recv() or similar 
//function shall still return this data.
/////////////////////////////////////////////////////////////////////////////////////////////////////

int ssl_socket_t::parse_http_headers()
{
  int recv_size; // size in bytes received or -1 on error 
  const int size_buf = 1024;
  char buf[size_buf];

  if ((recv_size = SSL_peek(m_ssl, buf, size_buf)) == -1)
  {
    return -1;
  }

  std::string str(buf);
  size_t pos = str.find("\r\n\r\n");

  if (pos == std::string::npos)
  {
    return -1;
  }

  std::string str_headers(str.substr(0, pos + 4));
  int header_len = static_cast<int>(pos + 4);
  std::cout << str_headers.c_str();

  //now get headers with the obtained size from socket
  if ((recv_size = SSL_read(m_ssl, buf, header_len)) == -1)
  {
    return -1;
  }

  //sanity check
  std::string str1(buf);
  assert(str1 == str);
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::receive
/////////////////////////////////////////////////////////////////////////////////////////////////////

int ssl_socket_t::receive()
{
  int len = 1024;
  char buf[1025];
  parse_http_headers();
  do
  {
    len = SSL_read(m_ssl, buf, 1024);
    buf[len] = 0;
    std::string str(buf);
    m_response += str;
    size_t pos = str.find("\n\r\n");
    if (pos != std::string::npos)
    {
      //found end of data
      break;
    }
    pos = str.find("\r\n\r\n");
    if (pos != std::string::npos)
    {
      //found end of message
      break;
    }
  } while (len > 0);
  if (len < 0)
  {
    int err = SSL_get_error(m_ssl, len);
    if (err == SSL_ERROR_WANT_READ)
      return 0;
    if (err == SSL_ERROR_WANT_WRITE)
      return 0;
    if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)
      return -1;
  }

  std::ofstream ofs("response.txt", std::ios::out | std::ios::binary);
  ofs.write(m_response.c_str(), m_response.size());
  ofs.close();
  return 0;
}


