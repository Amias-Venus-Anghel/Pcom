#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "parson.h"
#include <stdbool.h>
#include "helpers.h"
#include "requests.h"

#ifndef _RESPONSE_
#define _RESPONSE_

// returns a json formate with the username and password
char* get_user_input();
// prints the succes message or an error
void print_req_state(char* response, char* succes_message);
// returns the searched message or NULL if it's not found
char* find_msg(char* response, char* msg, int counter, char* del_end);
// returns a json formate with the new book or NULL if information is wrong
char* get_new_book();

#endif

