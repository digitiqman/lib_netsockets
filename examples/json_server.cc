#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctime>
#include "socket.hh"

unsigned short port = 2001;

///////////////////////////////////////////////////////////////////////////////////////
//main
///////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
#if defined (HAVE_JANSSON)
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
    char* buf;
    json_t *response = NULL;
    json_t *request = NULL;
    socket_t socket = server.accept_client();

    // convert IP addresses from a dots-and-number string to a struct in_addr and back
    str_ip = inet_ntoa(socket.m_sockaddr_in.sin_addr);
    std::cout << prt_time() << "server accepted: " << str_ip << " <" << socket.m_socket_fd << "> " << std::endl;

    if ((request = socket.read_json()) != NULL)
    {
      buf = json_dumps(request, JSON_PRESERVE_ORDER & JSON_COMPACT);
      std::cout << prt_time() << "server received: " << str_ip << " <" << socket.m_socket_fd << "> " << buf << std::endl;
      free(buf);
    }

    //get dates
    json_t *json_obj;
    json_obj = json_object_get(request, "start_year");
    json_int_t start_year = json_integer_value(json_obj);

    //do response
    response = json_object();
    json_object_set_new(response, "next_year", json_integer(start_year + 1));

    if (socket.write_json(response) > 0)
    {
      buf = json_dumps(response, JSON_PRESERVE_ORDER & JSON_COMPACT);
      std::cout << prt_time() << "server sent to: " << str_ip << " <" << socket.m_socket_fd << "> " << buf << std::endl;
      free(buf);
    }

    socket.close();
    json_decref(request);
    json_decref(response);
    std::cout << std::endl;
  }
  server.close();
  return 0;
#endif
}


