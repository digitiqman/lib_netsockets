#include <string>
#include <iostream>
#include "ssl_socket.hh"

int main(int argc, char *argv[])
{
  ssl_socket_t ssl;
  ssl.open("172.217.3.164");
  std::string request = 
    "GET https://www.google.com/finance/getprices?i=86400&p=1Y&f=d,o&df=cpct&q=.DJI  HTTP/1.1\r\n\r\n";
  ssl.send(request.c_str());
  ssl.receive();
  ssl.close_socket();
  return 0;
}
