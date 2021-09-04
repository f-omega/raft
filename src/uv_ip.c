#include <stdlib.h>
#include <string.h>

#include <uv.h>

#include "../include/raft.h"

#include "uv_ip.h"

int uvIpParse(const char *address, struct sockaddr_storage *addr)
{
    char buf[256];
    char *host;
    char *port;
    char *colon = ":";
    int rv;

    /* TODO: turn this poor man parsing into proper one */
    if ( *address == '[' ) {
      char *addrend;

      strcpy(buf, address);

      host = buf + 1;
      addrend = strchr(host, ']');

      if ( !addrend )
        return RAFT_NOCONNECTION;

      *addrend = 0;

      port = addrend + 1;
      if ( *port == 0 ) {
        port = "8080";
      } else if ( *port != ':' ) {
        return RAFT_NOCONNECTION;
      } else {
        port++;
      }

      rv = uv_ip6_addr(host, atoi(port), (struct sockaddr_in6 *) addr);
      if ( rv != 0 ) {
        return RAFT_NOCONNECTION;
      }
    } else {
      strcpy(buf, address);
      host = strtok(buf, colon);
      port = strtok(NULL, ":");
      if (port == NULL) {
        port = "8080";
      }

      rv = uv_ip4_addr(host, atoi(port), (struct sockaddr_in *) addr);
      if (rv != 0) {
        return RAFT_NOCONNECTION;
      }
    }

    return 0;
}
