#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "socket.hh"

#ifdef HAVE_GASON
#include "gason.hpp"
#include "jsonbuilder.hpp"
using namespace gason;
#endif

unsigned short port = 2001;

///////////////////////////////////////////////////////////////////////////////////////
//main
//TCP client that writes and reads JSON messages 
///////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
#if defined (HAVE_GASON)
  char server[255]; // server host name or IP

  strcpy(server, "127.0.0.1");

  for (int i = 1; i < argc && argv[i][0] == '-'; i++)
  {
    switch (argv[i][1])
    {
    case 's':
      strcpy(server, argv[i + 1]);
      i++;
      break;
    case 'p':
      port = atoi(argv[i + 1]);
      i++;
      break;
    }
  }

  //make JSON
  char buf_json[257] = { 0 };
  gason::JSonBuilder doc(buf_json, 256);
  doc.startObject();
  doc.addValue("start_year", 2017);
  doc.endObject();

  tcp_client_t client(server, port);
  std::cout << "client connecting to: " << server << ":" << port << " <" << client.m_socket_fd << "> " << std::endl;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //create socket and open connection
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  if (client.open() < 0)
  {
  }
  std::cout << "client connected to: " << server << ":" << port << " <" << client.m_socket_fd << "> " << std::endl;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //write request
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  if (client.write_json(buf_json) < 0)
  {
  }
  std::cout << "client sent: ";
  std::cout << buf_json << " " << server << ":" << port << " <" << client.m_socket_fd << "> " << std::endl;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //read response
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  std::string str_response = client.read_json();
  std::cout << "client received: ";
  std::cout << str_response << " " << server << ":" << port << " <" << client.m_socket_fd << "> " << std::endl;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //close connection
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  client.close();
#endif
  return 0;
}



