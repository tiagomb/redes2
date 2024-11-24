#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void create_client(struct client *c){
    c->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1){
        printf("Falha ao criar socket\n");
        exit(1);
    }
    c->servaddr.sin_family = AF_INET;
    c->servaddr.sin_addr.s_addr = ("192.168.1.2");
    c->servaddr.sin_port = htons(8080);
}

void send_messages(struct client *c, int num){
    long long int fim, inicio;
    int len;
    for (int i = 0; i < 1000; i++){
        c->buffer[i] = 0;
    }
    inicio = timestamp();
    printf ("Enviando %d pacotes para o servidor via UDP\n", num);
    for (int i = 0; i < num; i++){
        sendto(c->sockfd, c->buffer, sizeof(c->buffer), MSG_CONFIRM, (struct sockaddr*)&(c->servaddr), sizeof(c->servaddr));
    }
    fim = timestamp() - inicio;
    printf ("Pacotes enviados para o servidor\n", num);
    printf ("Tempo total: %ldmds\n", fim);
    printf ("Vazão: %.2f bytes", (float)(1000*num)/(float)fim);
}

void create_server(struct server *s){
    s->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        printf("Falha ao criar socket\n");
        exit(1);
    }
    s->servaddr.sin_family = AF_INET;
    s->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s->servaddr.sin_port = htons(8080);

    if (bind(s->sockfd, (struct sockaddr*)&(s->servaddr), sizeof(servaddr)) != 0){
        printf ("Bind falhou\n");
        exit(1);
    }
}

void receive_messages(struct server *s){
    long long int fim, inicio;
    int bytes;
    struct timeval timeout = {5000/1000, (5000%1000)*1000};
    setsockopt(s->sockfd, SOL_SOCKET, SO_RVCTIMEO, &timeout, sizeof(timeout));
    inicio = timestamp();
    printf ("Recebendo pacotes do cliente via UDP\n");
    while (1){
        bytes = read(s->connfd, s->buffer, sizeof(s->buffer));
        if (bytes == -1)
            break;
        s->cont++;
    }
    fim = timestamp() - inicio - 5000;
    printf ("%d\n pacotes recebidos do servidor\n", s->cont);
    printf ("Tempo total: %ldms\n", fim);
    printf ("Vazão: %.2f bytes", (float)(1000*num)/(float)fim);
}

int main(int argc, char* argv[]){
    int num, cflag, sflag;
    while ((c = getopt(argc, argv, "sc:")) != -1)
        switch(c){
            case 'c':
                if (sflag || cflag){
                    printf ("Já existe uma flag ativa\n");
                    exit(1);
                }
                cflag = 1;
                num = atoi(optarg);
                break;
            case 's':
                if (sflag || cflag){
                    printf ("Já existe uma flag ativa\n");
                    exit(1);
                }
                sflag = 1;
                break;
            default:
                printf ("Uso: ./UDP -c <num de pacotes> ou ./UDP -s\n");
                exit(1);
        }
    if (cflag){
        struct client c;
        create_client(&c);
        send_messages(&c, num);
    }
    else{
        struct server s;
        create_server(&s);
        receive_messages(&s);
    }
}