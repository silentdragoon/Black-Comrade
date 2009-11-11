#include "DiscoveryAgent.h"
#include <string>
#include <cstdio>
using namespace std;

int main(void){
  DiscoveryAgent *L;
  L = new DiscoveryAgent();
  
  char answer[2];

  printf("Would you like to be the server? (y/n) ");
  gets(answer);
  
  if (answer[0] == 'y' || answer[0] == 'Y')
  {
    L->beServer(6001,20);
  }
  else
  {
    while(1)
    {
      char* server = L->findServer(6001,6000,5);
      if (server == 0 || server[0] == 0)
      {
        printf("No servers found. Would you like to (q)uit or (c)ontinue as the server? ");
        gets(answer);
        if (answer[0] == 'q' || answer[0] == 'Q') break;
        L->beServer(6001,20);
        break;
      }
      else
      {
        printf("You chose %s as the server\n", server);
        break;
      }
    }
  }
  return 0;
}
