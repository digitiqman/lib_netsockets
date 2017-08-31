#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ssl_socket.hh"

///////////////////////////////////////////////////////////////////////////////////////
//main
//The URL format is: https://www.google.com/finance/getprices?i=[PERIOD]&p=[DAYS]d&f=d,o,h,l,c,v&df=cpct&q=[TICKER]
//Example: https://www.google.com/finance/getprices?i=3600&p=1d&f=d,o&df=cpct&q=.DJI
//[PERIOD] : Interval or frequency in seconds
//[DAYS] : The historical data period, where "10d" means that we need historical stock prices data for the past 10 days.
//[TICKER] : This is the ticker symbol of the stock

///////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  ssl_socket_t ssl;
  // address of google.com
  // Dow ".DJI"
  // SP500 ".INX" 
  // Nasdaq  ".IXIC"
  if (ssl.open(3600, 1, ".INX", "172.217.3.164") < 0)
  {
    std::cout << "connection error";
  }
  ssl.close();
  return 0;
}
