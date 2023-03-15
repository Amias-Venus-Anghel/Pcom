#include "response.h"


char* get_user_input() {
    // read username and password for login
    char user[100];
    char pword[100];

    printf("username=");
    fgets(user, 100, stdin);
    printf("password=");
    fgets(pword, 100, stdin);

    // remove endline character from input
    user[strlen(user) - 1] = '\0';
    pword[strlen(pword) - 1] = '\0';

    // create json information
    JSON_Value* info_val = json_value_init_object();
    JSON_Object* info_obj = json_value_get_object(info_val);
    json_object_set_string(info_obj, "username", user);
    json_object_set_string(info_obj, "password", pword);

    return json_serialize_to_string_pretty(info_val);
}

char* get_new_book() {
    // read username and password for login
    char title[100];
    char author[100];
    char genre[100];
    int pcount;
    char pub[100];

    printf("title=");
    fgets(title, 100, stdin);
    printf("author=");
    fgets(author, 100, stdin);
    printf("genre=" );
    fgets(genre, 100, stdin);
    printf("publisher=" );
    fgets(pub, 100, stdin);
    // check if a valid page number is introduced
    printf("page_count=");
    char check;
    if (scanf("%d%c", &pcount, &check) != 2 || check != '\n') {
        printf("ERROR: Invalid page count!\n");
       return NULL;
    }
    if (!strcmp(title, "\n") || !strcmp(author, "\n") || !strcmp(genre, "\n") ||
        !strcmp(pub, "\n")) {
        printf("ERROR: Invalid input!\n");
        return NULL;
    }


    // remove endline character from input
    title[strlen(title) - 1] = '\0';
    author[strlen(author) - 1] = '\0';
    genre[strlen(genre) - 1] = '\0';
    pub[strlen(pub) - 1] = '\0';


    // create json information
    JSON_Value* info_val = json_value_init_object();
    JSON_Object* info_obj = json_value_get_object(info_val);
    json_object_set_string(info_obj, "title", title);
    json_object_set_string(info_obj, "author", author);
    json_object_set_string(info_obj, "genre", genre);
    json_object_set_number(info_obj, "page_count", pcount);
    json_object_set_string(info_obj, "publisher", pub);

    return json_serialize_to_string_pretty(info_val);
}

char* find_msg(char* response, char* msg, int counter, char* del_end) {
    // counter = number of possition to be added for the start of the message
    // del_end = delimitator to find the end of the message

    char* index = strstr(response, msg);
    // if error is found
    if (index != NULL) {
        // begining of error is at index + counter
        char* end = strstr(response + (index + counter - response), del_end);
        int size = end - index - counter;

        // extract error from response
        char* wanted_msg = (char*) calloc(size + 1, sizeof(char));
        strncpy(wanted_msg, response + (index + counter - response), size);
        return wanted_msg;
    }

    return NULL;
}

void print_req_state(char* response, char* succes_message) {
    char* err_msg = find_msg(response, "error", 8, "\"");
    if (!err_msg) {
        printf("%s\n", succes_message);
    } else {
        printf("ERROR! %s\n", err_msg);
    }
}
