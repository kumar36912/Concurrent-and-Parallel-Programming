/* #includes are here */

int main() {
  signal(SIGCHLD, SIG_IGN); /* avoid zombies */                           /** line 1 **/

  char buffer[BUFF_SIZE + 1];      
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int sock = create_server_socket(false);                                 /** line 2 **/

  /* connections + requests */
  while (true) {                                                          /** line 3 **/
     int client = accept(sock, 
			(struct sockaddr*) &client_addr, 
			&len);
    if (client < 0) error_msg("Problem with accept call", true);
    announce_client(&client_addr.sin_addr);                               /** line 4 **/

    /* fork child */
    pid_t pid = fork();                                                   /** line 5 **/
    if (pid < 0) error_msg("Problem with fork call", false);

    /* 0 to child, child's PID to parent */
    if (0 == pid) {  /** child **/                                        /** line 6 **/
      close(sock);   /* child's listening socket */                       /** line 7 **/

      /* request */
      bzero(buffer, sizeof(buffer));                                      /** line 8 **/
      int bytes_read = recv(client, buffer, sizeof(buffer), 0);           /** line 9 **/
      if (bytes_read < 0) error_msg("Problem with recv call", false);
      
      /* response */
      char response[BUFF_SIZE * 2]; 
      bzero(response, sizeof(response));
      generate_echo_response(buffer, response);
      int bytes_written = send(client, response, strlen(response), 0);    /** line 10 **/
      if (bytes_written < 0) error_msg("Problem with send call", false);

      close(client);                                                      /** line 11 **/
      exit(0);       /* terminate */                                      /** line 12 **/
    } 
    else             /** parent **/
      close(client); /* parent's read/write socket. */                    /** line 13 **/
  } 
  return 0; 
}
