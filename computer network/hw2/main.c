// hw2 20181283 HWANG GYOHUN
// Implementing an HTTP proxy
/*  Accept an absolute HTTP requests from clients
        GET http://www.cs.princeton.edu/index.html HTTP/1.0
    Forward requests to origin servers (with relative URLs)
        GET /index.html HTTP/1.0
        Host: www.cs.princeton.edu
        Connection: close (need to add this header or change to this if needed) (other headers can be just forwarded)
    Return response data to a client
    
    < cases for invalid >
    An invalid request from the client should be answered with an error code, i.e. "Bad Request" (400)
        or "Not Implemented" (501) for valid HTTP methods other than GET

    Should run using a command like “proxy <port number>”

    1. Accept
    2. Forward -> proxy server로
    3. return response data

*/
#include <netdb.h>
#include <arpa/inet.h>


#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h> // for strlen

#define len 500 // absolute HTTP request max size
#define size 5000 // size for msg.
#define origin_port 80
int is_valid(char abs_req[]);

int main(int argc, char* argv[]){
    int port_number = atoi(argv[1]);
    // declare socket
    int proxy_sock;
    int client_sock;
    // declare ip/port
    struct sockaddr_in client;
    struct sockaddr_in proxy;

    memset(&proxy,0,sizeof(proxy));
    proxy.sin_family = AF_INET;
    proxy.sin_port = htons(port_number);
    proxy.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 1. socket()
    proxy_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(proxy_sock < 0){
        printf("CAN'T SOCKET\n");
        return 1;
    }
    // Reusing the same port ???
    // 2. bind()
    if(bind(proxy_sock, (struct sockaddr*)&proxy, sizeof(proxy)) < 0){
        printf("CAN'T BIND\n");
        return 1;
    }
    // 3. listen()
    if(listen(proxy_sock, 32) < 0){
        printf("CAN'T LISTEN");
        return 1;
    }
    // 4. accept()
    socklen_t client_size = sizeof(client);
    client_sock = accept(proxy_sock, (struct sockaddr*)&client, &client_size);
    if(client_sock < 0){
        printf("CAN'T ACCEPT\n");
        return 1;
    }
    // 5. read/receive
    char client_buffer[4096];
    int expected_data_len = sizeof(client_buffer);
    int client_receive = recv(client_sock, client_buffer, expected_data_len, 0);
    if(client_receive < 0){
        printf("RECEIVE ERROR\n");
        return 1;
    }
    unsigned int i = 0;
    while(client_buffer[i] != '\n'){
        i++;
    }
    client_buffer[i] = '\0';
    int valid = is_valid(client_buffer);
    if(valid == 0){
        char *ptr = strtok(client_buffer, " ");
        char *origin_server;
        char *relative_url;
        if(ptr != NULL) origin_server = (ptr = strtok(NULL, " "));
        else return 1;
        if(ptr != NULL) relative_url = (ptr = strtok(NULL, "\n"));
        else return 1;
        origin_server = strchr(origin_server, 'w');
        
        // GET http://www.gnu.org HTTP/1.0
        // GET https://www.gnu.org HTTP/1.0
        // GET http://www.gnu.org/ HTTP/1.0
        //  -> www.gnu.org
        //  -> HTTP/1.0
        char send_msg[4096] = "";
        strcat(send_msg, "GET ");
        strcat(send_msg, relative_url);
        strcat(send_msg, "\nHost: ");
        strcat(send_msg, origin_server);
        strcat(send_msg, "\nConnection: close\n\n");

        struct sockaddr_in origin;
        struct hostent *server;
        if((server = gethostbyname(origin_server)) == NULL){
            printf("SERVER ERROR\n");
            return 1;
        }
        memset(&origin,0,sizeof(origin));
        origin.sin_port = htons(origin_port);
        origin.sin_family = AF_INET;
        origin.sin_addr.s_addr = *((unsigned long *)server->h_addr);
        int send_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(send_sock < 0){
            printf("CAN'T SEND_SOCK\n");
            return 1;
        }
        if (connect(send_sock, (struct sockaddr*)&origin, sizeof(origin)) < 0) {
            printf("CAN'T CONNECT\n");
            return 1;
        }
        int send_msg_size = sizeof(send_msg);
        int send_byte = send(send_sock, send_msg, send_msg_size, 0);
        if(send_byte < 0){
            printf("SEND ERRER\n");
            return 1;
        }
        char origin_buffer[4096] = "";
        expected_data_len = sizeof(origin_buffer);
        int origin_bytes = recv(send_sock, origin_buffer, expected_data_len, 0);
        if(origin_bytes < 0){
            printf("RECIEVE ERROR\n");
            return 1;
        }
        // 6. write
        // printf("%s\n", origin_buffer);
        write(client_sock, origin_buffer, expected_data_len);
        close(send_sock);
    }
    else if(valid == 1)
    write(client_sock, "Bad Request(400)\n", sizeof("Bad Request(400)\n"));
    else
    write(client_sock, "Not Implemented(501)\n", sizeof("Not Implemented(501)\n"));

    // 8. close
    close(client_sock);
    close(proxy_sock);

    return 0;
}



int is_valid(char abs_req[]){
    char right_req[11];
    right_req[0] = 'G';
    right_req[1] = 'E';
    right_req[2] = 'T';
    right_req[3] = ' ';
    right_req[4] = 'h';
    right_req[5] = 't';
    right_req[6] = 't';
    right_req[7] = 'p';
    right_req[8] = ':';
    right_req[9] = '/';
    right_req[10] = '/';
    // EX)  GET http://www.gnu.org/ HTTP/1.0
    //      GET http://www.gnu.org HTTP/1.0
    if(strlen(abs_req) < sizeof(right_req)){
        return 1;
    }
    if(!(abs_req[0] == 'G' && abs_req[1] == 'E' && abs_req[2] == 'T')){
        return 2;
    }
    if(abs_req[9] == 's'){
        return 2;
    }
    for(int i = 0; i < sizeof(right_req); i ++){
        if(abs_req[i] != right_req[i]){
            return 1;
        }
    }
    return 0;
}