#include "response.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;
    char user_req[35]; // used to read the request of the user
                        // from standard input
    char* cookie; // remember cookie from login
    char* tokenJWT; // remeber autorization token

    // initializing
    cookie = NULL;
    tokenJWT = NULL;

    while (1) {
        sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
        memset(user_req, 0, 35);
        fgets(user_req, 35, stdin);

        // exit
        if (!strncmp(user_req, "exit", 4)) {
            close_connection(sockfd);
            return 0;
        }
        // register
        else if (!strncmp(user_req, "register", 8)) {
            message =  compute_post_request("34.241.4.235", "/api/v1/tema/auth/register",
                         "application/json", get_user_input() , NULL, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            print_req_state(response, "Register succesful!");
        }
        // login
        else if (!strncmp(user_req, "login", 5)) {
            message =  compute_post_request("34.241.4.235", "/api/v1/tema/auth/login",
                         "application/json", get_user_input() , NULL, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            print_req_state(response, "Login succesful!");

            cookie = find_msg(response, "Set-Cookie", 12, ";");
            if (!cookie)
                continue;
            tokenJWT = NULL;
            printf("Cookie: %s\n", cookie);
        }
        // enter_library
        else if (!strncmp(user_req, "enter_library", 13)) {
             if (!cookie) {
                printf("NO ACCESS: Login first!\n");
                continue;
            }
            message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access",
                                            NULL, cookie, NULL, false);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            tokenJWT = find_msg(response, "token", 8, "\"");
            if(!tokenJWT) {
                print_req_state(response, "You don't have access!");
                continue;
            }
            print_req_state(response, "You have access!");
            printf("Token: %s\n", tokenJWT);
        }
        // get_books
        else if (!strncmp(user_req, "get_books", 9)) {
            if (!cookie) {
                printf("NO ACCESS: Login first!\n");
                continue;
            }
            if (!tokenJWT) {
                printf("NO ACCESS: Enter library first!\n");
                continue;
            }
            message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books",
                                            NULL, cookie, tokenJWT, false);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char* books = find_msg(response, "[{", 1, "]");
            if(!books) {
                print_req_state(response, "No books to show.");
                continue;
            }
            print_req_state(message, books);
        }
        // get_book
        else if (!strncmp(user_req, "get_book", 8)) {
            if (!cookie) {
                printf("NO ACCESS: Login first!\n");
                continue;
            }
            if (!tokenJWT) {
                printf("NO ACCESS: Enter library first!\n");
                continue;
            }
            printf("id=");
            int id;
            char check;
            if (scanf("%d%c", &id, &check) != 2 || check != '\n') {
                printf("Invalid id!\n");
                continue;
            }

            char pathway[31];
            sprintf(pathway, "/api/v1/tema/library/books/%d", id);

            message = compute_get_request("34.241.4.235", pathway, NULL, cookie, tokenJWT, false);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            char* book = find_msg(response, "[{", 1, "]");
            print_req_state(response, book);
        }
        // add_book
        else if (!strncmp(user_req, "add_book", 8)) {
            if (!cookie) {
                printf("NO ACCESS: Login first!\n");
                continue;
            }
            if (!tokenJWT) {
                printf("NO ACCESS: Enter library first!\n");
                continue;
            }
            char* msg = get_new_book();
            if (!msg) {
                // the input was invalid
                printf("Book was not added!\n");
                continue;
            }
            message =  compute_post_request("34.241.4.235", "/api/v1/tema/library/books",
                         "application/json", msg, cookie, tokenJWT);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            print_req_state(response, "Book added!");
        }
        // delete
        else if (!strncmp(user_req, "delete_book", 11)) {
            if (!cookie) {
                printf("NO ACCESS: Login first!\n");
                continue;
            }
            if (!tokenJWT) {
                printf("NO ACCESS: Enter library first!\n");
                continue;
            }
            printf("id=");
            int id;
            char check;
            if (scanf("%d%c", &id, &check) != 2 || check != '\n') {
                printf("Invalid id!\n");
                continue;
            }

            char pathway[31];
            sprintf(pathway, "/api/v1/tema/library/books/%d", id);

            message = compute_get_request("34.241.4.235", pathway, NULL, cookie, tokenJWT, true);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            print_req_state(response, "Book deleted!");
        }
        // logout
        else if (!strncmp(user_req, "logout", 6)) {
            message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout",
                                            NULL, cookie, tokenJWT, false);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            print_req_state(response, "Logout successful!");
            tokenJWT = NULL;
            cookie = NULL;
        }
        // invalid request
        else {
            printf("Invalid request!\n");
        }


    }
    return 0;
}
