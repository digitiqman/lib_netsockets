#include "ssl_socket.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::ssl_socket_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

ssl_socket_t::ssl_socket_t()
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
  SSL_shutdown(m_ssl);
  SSL_free(m_ssl);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::close()
/////////////////////////////////////////////////////////////////////////////////////////////////////

void ssl_socket_t::close()
{
#if defined (_MSC_VER)
  closesocket(m_socket_fd);
#else
  ::close(m_socket_fd);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::open
/////////////////////////////////////////////////////////////////////////////////////////////////////

int ssl_socket_t::open(int period, int days, std::string ticker, const char *str_ip)
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


  //The URL format is: https://www.google.com/finance/getprices?i=[PERIOD]&p=[DAYS]d&f=d,o,h,l,c,v&df=cpct&q=[TICKER]
  //Example: https://www.google.com/finance/getprices?i=3600&p=1d&f=d,o&df=cpct&q=.DJI
  //[PERIOD] : Interval or frequency in seconds
  //[DAYS] : The historical data period, where "10d" means that we need historical stock prices data for the past 10 days.
  //[TICKER] : This is the ticker symbol of the stock

  std::string request;
  request = "GET https://www.google.com/finance/getprices?i=";
  request += std::to_string(period);
  request += "&p=";
  request += std::to_string(days);;
  request += "d&f=d,o&df=cpct&q=";
  request += ticker;
  request += "  HTTP/1.1\r\n\r\n";

  send(request.c_str());
  parse_http_headers();
  receive();
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//ssl_socket_t::send
/////////////////////////////////////////////////////////////////////////////////////////////////////

int ssl_socket_t::send(const char *buf)
{
  std::cout << buf;
  int sent_size;
  sent_size = SSL_write(m_ssl, buf, strlen(buf));
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
  std::cout << str_headers.c_str() << std::endl;

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
//SSL_read blocks; here we detect end of message to exit
/////////////////////////////////////////////////////////////////////////////////////////////////////

int ssl_socket_t::receive()
{
  size_t pos;
  int len = 1024;
  char buf[1000000];
  std::string str_all;
  do
  {
    len = SSL_read(m_ssl, buf, 1024);
    buf[len] = 0;
    std::cout << buf;
    std::string str(buf);
    str_all += str;
    pos = str.find("\n\r\n");
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

  pos = str_all.find("TIMEZONE_OFFSET");
  pos = str_all.find("\n", pos + 1);
  pos = str_all.find("\n", pos + 1);
  //data starts here
  size_t pos_end = str_all.find("\n\r\n");
  //data ends here
  std::string str_data = str_all.substr(pos + 1, pos_end - pos);
  pos = 0;
  std::string str_rec;
  size_t pos_0; //previous position
  while (true)
  {
    pos_0 = pos;
    pos = str_data.find("\n", pos + 1);
    if (pos == std::string::npos)
    {
      //found end of data
      break;
    }

    str_rec = str_data.substr(pos_0, pos - pos_0);
    size_t pos_c = str_rec.find(",");
    std::string str_time = str_rec.substr(0, pos_c);
    std::string str_value = str_rec.substr(pos_c + 1);
    std::cout << str_time.c_str() << " " << str_value.c_str() << std::endl;
    time_value_t tv(std::stoi(str_time), std::stof(str_value));
    m_tv.push_back(tv);
  }
  return 0;
}


