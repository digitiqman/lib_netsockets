#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "http.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//usage
/////////////////////////////////////////////////////////////////////////////////////////////////////

void usage()
{
  std::cout << "usage: http_client -s SERVER <-p PORT> <-g> <-f /FILE> <-v> <-h>" << std::endl;
  std::cout << "-s SERVER: fully qualified web server name (default 127.0.0.1)" << std::endl;
  std::cout << "-p PORT: server port (default 3000)" << std::endl;
  std::cout << "-g Use GET of -f FILE, otherwise POST is used" << std::endl;
  std::cout << "-f FILE: file located at web server root; file path name must start with '/' (default index.html)" << std::endl;
  std::cout << "-v: verbose, output of retrieved file is printed" << std::endl;
  std::cout << "-h: help, exit" << std::endl;
  exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////
//main
//HTTP client 
///////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  const char *host_name = "127.0.0.1"; // server name 
  const char *path_name = "index.html"; // name of file to retrieve
  unsigned short port = http_port;
  bool verbose = false;
  bool get = false;

  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      switch (argv[i][1])
      {
      case 'h':
        usage();
        break;
      case 'v':
        verbose = true;
        break;
      case 'g':
        get = true;
        break;
      case 's':
        host_name = argv[i + 1];
        i++;
        break;
      case 'f':
        path_name = argv[i + 1];
        i++;
        break;
      case 'p':
        port = atoi(argv[i + 1]);
        i++;
        break;
      default:
        usage();
      }
    }
    else
    {
      usage();
    }
  }

  http_t client(host_name, port);

  //open connection
  if (client.open() < 0)
  {

  }

  std::cout << "client connected to: " << host_name << ":" << port << std::endl;

  if (get)
  {
    client.get(path_name, verbose);
  }
  else
  {
    std::string str_body("start_year=2016&end_year=2017");
    client.post(str_body);
  }
  client.close();
  return 0;
}

