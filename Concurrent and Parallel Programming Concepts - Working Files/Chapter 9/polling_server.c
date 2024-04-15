#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include "utils.h"

#define MAX_BUFFERS (BACKLOG + 1) /* max clients + listener */

int main() {
  char buffer[BUFF_SIZE + 1];      
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int sock = create_server_socket(true); /* non-blocking */              /** line 1 **/

  struct epoll_event event,     /* server2epoll */                       /** line 2 **/
    event_buffers[MAX_BUFFERS]; /* epoll2server */                       /** line 3 **/  
  int epollfd = epoll_create1(0); 
  if (epollfd < 0) error_msg("Problem with epoll_create", true);

  /* Register the listening-for-clients event. */
  event.events = EPOLLIN;  /* incoming == connections */                 /** line 4 **/
  event.data.fd = sock;    /* register listener */                       /** line 5 **/
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &event) < 0)               /** line 6 **/
    error_msg("Problem with epoll_ctl call", true);
  
  /* Await connections + requests. */
  while (true) {
    /* event count: epoll_wait is a blocking call */
    int n = epoll_wait(epollfd, event_buffers, MAX_BUFFERS, -1);         /** line 7 **/
    if (n < 0) error_msg("Problem with epoll_wait call", true);

    /*
       -- If connection, register for request events: may be none or more connections
       -- If request, read and echo 
    */
    int i;
    for (i = 0; i < n; i++) {      
      /* connections? */
      if (event_buffers[i].data.fd == sock) {                            /** line 8 **/
	while (true) {
	  socklen_t len = sizeof(client_addr);
	  int client = accept(sock,                                      /** line 9 **/
			      (struct sockaddr *) &client_addr, 
			      &len);
	  /* no client? */
	  if (client < 0 && (EAGAIN == errno || EWOULDBLOCK == errno))   /** line 10 **/
	    break;
	  
	  /* client */
	  fcntl(client, F_SETFL, O_NONBLOCK); /* non-blocking */         /** line 11 **/
	  event.events = EPOLLIN;             /* incoming == request */  /** line 12 **/
	  event.data.fd = client;                                        /** line 13 **/
	  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client, &event) < 0)
	    error_msg("Problem with epoll_ctl ADD call", false);	  
	  announce_client(&client_addr.sin_addr);
	}
      }
      
      /* request */
      else {                                                             /** line 14 **/
	bzero(buffer, sizeof(buffer));
	int bytes_read = recv(event_buffers[i].data.fd,                  /** line 15 **/
			      buffer, 
			      sizeof(buffer), 
			      0); 

	/* echo request */
	if (bytes_read > 0) {
	  char response[BUFF_SIZE * 2]; 
	  bzero(response, sizeof(response));
	  generate_echo_response(buffer, response);
	  int bytes_written =  send(event_buffers[i].data.fd,            /** line 16 **/
				    response, 
				    strlen(response), 
				    0); 
	  if (bytes_written < 0) 
	    error_msg("Problem with send call", false);
	  close(event_buffers[i].data.fd); /* epoll stops polling fd */  /** line 17 **/
	}  
      } 
    } 
  } 
  return 0;
}

