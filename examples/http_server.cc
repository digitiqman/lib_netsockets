#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "socket.hh"

bool verbose = true;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//usage
/////////////////////////////////////////////////////////////////////////////////////////////////////

void usage()
{
  std::cout << "-p PORT: server port (default 3000)" << std::endl;
  std::cout << "-v: verbose output" << std::endl;
  std::cout << "-h: help, exit" << std::endl;
  exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////
//main
///////////////////////////////////////////////////////////////////////////////////////

int handle_client(socket_t& socket);

int main(int argc, char *argv[])
{
  unsigned short port = 3000;

  for (int i = 1; i < argc && argv[i][0] == '-'; i++)
  {
    switch (argv[i][1])
    {
    case 'h':
      usage();
      break;
    case 'v':
      verbose = true;
      break;
    case 'p':
      port = atoi(argv[i + 1]);
      i++;
      break;
    }
  }

  tcp_server_t server(port);
  std::cout << "server listening on port " << port << std::endl;
  while (true)
  {
    socket_t socket = server.accept_client();

    // convert IP addresses from a dots-and-number string to a struct in_addr
    char *str_ip = inet_ntoa(socket.m_sockaddr_in.sin_addr);
    std::cout << prt_time() << "server accepted: " << str_ip << "," << socket.m_socket_fd << std::endl;

    if (handle_client(socket) < 0)
    {
      std::cout << "error on client handling";
    }
    socket.close();
  }
  server.close();
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//handle_client
/////////////////////////////////////////////////////////////////////////////////////////////////////

int handle_client(socket_t& socket)
{
  std::string http_headers;
  int recv_size; // size in bytes received or -1 on error 
  const int size_buf = 4096;
  char buf[size_buf];

  if (socket.parse_http_headers(http_headers) < 0)
  {
    std::cout << "parse_http_headers error\n";
    return -1;
  }

  unsigned long long size_boby = http_extract_field("Content-Length: ", http_headers);

  if (verbose)
  {
    std::cout << "received: Content-Length: " << size_boby << std::endl;
  }

  //now get body using size of Content-Length
  int len_recv = (int)size_boby;
  if ((recv_size = recv(socket.m_socket_fd, buf, len_recv, 0)) == -1)
  {
    std::cout << "recv error: " << strerror(errno) << std::endl;
    return -1;
  }

  std::string str_body(buf);
  str_body.resize(len_recv);

  if (verbose)
  {
    std::cout << "received: " << str_body << std::endl;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //response
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  std::string str_response("HTTP/1.1 200 OK\r\n\r\n");
  str_response += "<html><body><h1>Server</h1></body></html>";

  if (verbose)
  {
    std::cout << str_response << std::endl;
  }

  if (socket.write(str_response.c_str(), str_response.size()) < 0)
  {
    std::cout << "write response error\n";
    return -1;
  }

  return 0;
}


