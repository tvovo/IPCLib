#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>  
#include <sys/ioctl.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <netdb.h>  
#include <net/if.h>  
#include <string.h>
#include "utils.h"

const char* get_local_ip(const char* ethname)
{

  int socket_fd;
  struct sockaddr_in *sin;
  struct ifreq ifr;
  static char buffer[16] = {0};
  
  socket_fd = socket(AF_INET,SOCK_DGRAM,0); 
  if(socket_fd ==-1)
  {
    perror("socket error!\n");
    return NULL;
  }

  strcpy(ifr.ifr_name, ethname);
   
  if(ioctl(socket_fd, SIOCGIFADDR, &ifr) < 0)
  {
    perror("ioctl error\n");
    return NULL;
  }
  sin = (struct sockaddr_in *)&(ifr.ifr_addr);
  strcpy(buffer, inet_ntoa(sin->sin_addr));
  return buffer;
}
