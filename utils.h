#ifndef ___UTILS___
#define ___UTILS___

#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct client {
    struct sockaddr_in servaddr;
    int sockfd;
    char buffer[1000];
};

struct server {
    struct sockaddr_in servaddr, cli;
    int sockfd;
    int connfd;
    int cont;
    char buffer[1000];
};

long long int timestamp();

void create_client(struct client *c);

void send_messages(struct client *c, int num);

void create_server(struct server *s);

void receive_messages(struct server *s, int num);

#endif // ___UTILS___