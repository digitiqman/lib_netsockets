#define _CRT_NONSTDC_NO_DEPRECATE
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctime>
#include "socket.hh"

#ifdef HAVE_GASON
#include "gason.hpp"
#include "jsonbuilder.hpp"
using namespace gason;
#endif

unsigned short port = 2001;

///////////////////////////////////////////////////////////////////////////////////////
//main
///////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
#if defined (HAVE_GASON)
  std::string argv_str(argv[0]);
  std::string path = argv_str.substr(0, argv_str.find_last_of("/"));

  for (int i = 1; i < argc && argv[i][0] == '-'; i++)
  {
    switch (argv[i][1])
    {
    case 'p':
      port = atoi(argv[i + 1]);
      i++;
      break;
    case 'd':
      set_daemon(path.c_str());
      break;
    }
  }

  tcp_server_t server(port);
  std::cout << "server listening on port " << port << std::endl;
  while (true)
  {
    char *str_ip;
    socket_t socket = server.accept_client();

    // convert IP addresses from a dots-and-number string to a struct in_addr and back
    str_ip = inet_ntoa(socket.m_sockaddr_in.sin_addr);
    std::cout << prt_time() << "server accepted: " << str_ip << " <" << socket.m_socket_fd << "> " << std::endl;
    
    std::string str_request = socket.read_json();
    std::cout << prt_time() << "server received: " << str_request << std::endl;
    char* buf_request = strdup(str_request.c_str());

    //get dates
    JsonAllocator allocator;
    JsonValue root;
    JsonParseStatus status = jsonParse(buf_request, root, allocator);

    if (status != JSON_PARSE_OK)
    {

    }

    gason::JsonValue value = root.child("start_year");
    int start_year = value.toInt();

    //do response
    char buf_json[257] = { 0 };
    gason::JSonBuilder doc(buf_json, 256);

    doc.startObject();
    doc.addValue("next_year", start_year + 1);
    doc.endObject();

    if (socket.write_json(buf_json) < 0)
    {
    }

    std::cout << prt_time() << "server sent: " << buf_json << std::endl;
    socket.close_socket();
    free(buf_request);
  }
  server.close_socket();
  return 0;
#endif
}


