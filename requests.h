#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET/DELETE request string (query_params
// and cookies and tokJWT can be set to NULL if not needed)
char *compute_get_request(char* host, char* url, char* query_params,
							char* cookies, char* tokJWT, bool delete);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(char* host, char* url, char* content_type, char* body_data,
							char* cookies, char* tokJWT);
#endif
