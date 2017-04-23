#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "socket.hh"

unsigned short port = 2001;

///////////////////////////////////////////////////////////////////////////////////////
//main
//TCP client that writes and reads JSON messages 
///////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
#if defined (HAVE_JANSSON)
  char buf_server[255]; // server host name or IP
  char* buf = NULL;
  std::string str_response;

  strcpy(buf_server, "127.0.0.1");

  for (int i = 1; i < argc && argv[i][0] == '-'; i++)
  {
    switch (argv[i][1])
    {
    case 's':
      strcpy(buf_server, argv[i + 1]);
      i++;
      break;
    case 'p':
      port = atoi(argv[i + 1]);
      i++;
      break;
    }
  }


  //make JSON
  json_t *request = json_object();
  json_object_set_new(request, "start_year", json_integer(2016));

  json_dump_file(request, "request.json", 0);

  tcp_client_t client(buf_server, port);
  std::cout << "client connecting to: " << buf_server << ":" << port << " <" << client.m_socket_fd << "> " << std::endl;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //create socket and open connection
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  if (client.open() < 0)
  {
    std::string  str = "connect error to: ";
    str += buf_server;
    std::cout << str << ":" << port << std::endl;
    json_decref(request);
    return 1;
  }
  std::cout << "client connected to: " << buf_server << ":" << port << " <" << client.m_socket_fd << "> " << std::endl;

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //write request
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  if (client.write_json(request) < 0)
  {
    std::string  str = "send error to: ";
    str += buf_server;
    std::cout << str << ":" << port << std::endl;
    json_decref(request);
    return 1;
  }

  std::cout << "client sent: ";

  buf = json_dumps(request, 0);
  std::cout << buf << " " << buf_server << ":" << port << " <" << client.m_socket_fd << "> " << std::endl;
  free(buf);

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //read response
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  json_t *response = client.read_json();

  if (NULL == response)
  {
    std::string  str = "recv error from: ";
    str += buf_server;
    std::cout << str << ":" << port << std::endl;
    json_decref(request);
    return 1;
  }

  std::string str_response_name("response");
  if (str_response.size())
  {
     str_response_name += str_response;
  }
  str_response_name += ".json";
  json_dump_file(response, str_response_name.c_str(), 0);

  std::cout << "client received: ";

  buf = json_dumps(response, 0);
  std::cout << buf << " " << buf_server << ":" << port << " <" << client.m_socket_fd << "> " << std::endl;
  free(buf);

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //close connection
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  client.close();
#endif
  return 0;
}



