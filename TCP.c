#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void create_client(struct client *c){
    c->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (c->sockfd == -1){
        printf("Falha ao criar socket\n");
        exit(1);
    }
    c->servaddr.sin_family = AF_INET;
    inet_aton("192.168.1.2", &(c->servaddr.sin_addr));
    c->servaddr.sin_port = htons(8080);
    if (connect(c->sockfd, (struct sockaddr*)&(c->servaddr), sizeof(c->servaddr)) != 0){
        printf ("Conexão falhou");
        exit(1);
    }
}

void send_messages(struct client *c, int num){
    long long int fim, inicio;
    int bytes;
    for (int i = 0; i < 1000; i++){
        c->buffer[i] = 0;
    }
    inicio = timestamp();
    printf ("Enviando %d pacotes para o servidor via TCP\n", num);
    for (int i = 0; i < num; i++){
        bytes = write(c->sockfd, c->buffer, sizeof(c->buffer));
    }
    fim = timestamp() - inicio;
    printf ("Pacotes enviados para o servidor\n");
    printf ("Tempo total: %lldms\n", fim);
    printf ("Vazão: %.2f bytes", (float)(1000*num)/(float)fim);
}

void create_server(struct server *s){
    s->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (s->sockfd == -1){
        printf("Falha ao criar socket\n");
        exit(1);
    }
    s->servaddr.sin_family = AF_INET;
    s->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s->servaddr.sin_port = htons(8080);
    if (bind(s->sockfd, (struct sockaddr*)&(s->servaddr), sizeof(s->servaddr)) != 0){
        printf ("Bind falhou\n");
        exit(1);
    }
    if ((listen(s->sockfd, 5) != 0)){
        printf ("Escuta falhou\n");
        exit(1);
    }
    socklen_t len = sizeof(s->cli);
    s->connfd = accept(s->sockfd, (struct sockaddr*)&(s->cli), &len);
    if (s->connfd < 0){
        printf ("Conexão falhou\n");
        exit(1);
    }
}

void receive_messages(struct server *s, int num){
    int bytes;
    struct timeval timeout = {5000/1000, (5000%1000)*1000};
    setsockopt(s->connfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    printf ("Recebendo pacotes do cliente via TCP, %d pacotes são esperados\n", num);
    while (1){
        bytes = read(s->connfd, s->buffer, sizeof(s->buffer));
        if (bytes == -1)
            break;
        s->cont++;
    }
    printf ("%d\n pacotes recebidos do cliente\n", s->cont);
    printf ("%.2f%% dos pacotes foram recebidos\n", (float)(s->cont*100)/(float)num);
}

int main(int argc, char* argv[]){
    int num = 0, cflag = 0, sflag = 0, c = 0;
    while ((c = getopt(argc, argv, "s:c:")) != -1)
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
                num = atoi(optarg);
                sflag = 1;
                break;
            default:
                printf ("Uso: ./TCP -c <num de pacotes> ou ./TCP -s <num de pacotes>\n");
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
        receive_messages(&s, num);
    }
}