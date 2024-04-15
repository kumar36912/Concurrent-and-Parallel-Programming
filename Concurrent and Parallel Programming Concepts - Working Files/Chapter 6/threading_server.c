/* includes */

void* handle_client(void* client_ptr) { /** thread routine **/            /** line 1 **/
  pthread_detach(pthread_self()); /* terminates on return */              /** line 2 **/

  int client = *((int*) client_ptr); /* the socket */                     /** line 3 **/

  /* request */                                                           /** line 4 **/
  char buffer[BUFF_SIZE + 1];
  bzero(buffer, sizeof(buffer));
  int bytes_read = recv(client, buffer, sizeof(buffer), 0); 
  if (bytes_read < 0) error_msg("Problem with recv call", false);

  /* response */                                                          /** line 5 **/
  char response[BUFF_SIZE * 2]; 
  bzero(response, sizeof(response));
  generate_echo_response(buffer, response);
  int bytes_written = send(client, response, strlen(response), 0); 
  if (bytes_written < 0) error_msg("Problem with send call", false);

  close(client);                                                          /** line 6 **/
  return NULL;                                                            /** line 7 **/
} /* detached thread terminates on return */

int main() {  
  char buffer[BUFF_SIZE + 1];      
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int sock = create_server_socket(false);

  /* Accept connections */
  while (true) {
    int client = accept(sock, 
			(struct sockaddr*) &client_addr, 
			&len);
    if (client < 0) error_msg("Problem accepting a client request", true);
    announce_client(&client_addr.sin_addr);

    /* client handler */
    pthread_t tid;                                                        /** line 8 **/
    int flag = pthread_create(&tid,          /* id */                     /** line 9 **/
			      NULL,          /* attributes */
			      handle_client, /* routine */
			      &client);      /* routine's arg */
    if (flag < 0) error_msg("Problem creating thread", false);
  } 
  return 0; 
}
