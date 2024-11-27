#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

void create_client(struct client *c){
    c->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (c->sockfd == -1){
        printf("Falha ao criar socket\n");
        exit(1);
    }
    c->servaddr.sin_family = AF_INET;
    inet_aton(SERVER_ADDRESS, &(c->servaddr.sin_addr));
    c->servaddr.sin_port = htons(8080);
}

void print_client_data(struct client *c, int num, long long int fim, int nocheck){
    int bytes = num * MAX_SIZE;
    char space = ' ';
    FILE *arq = NULL;

    if (nocheck)
        arq = fopen("Client_UDP_NOCHECK.txt", "a");
    else
        arq = fopen("Client_UDP.txt", "a");
    
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arq, "%d", bytes);
    fprintf(arq, " ");    
    fprintf(arq, "%lld", fim);
    fprintf(arq, "\n");

}

void print_server_data(struct server *s, int num, int nocheck){
    int bytes = num * MAX_SIZE;
    char space = ' ';
    FILE *arq = NULL;

    if (nocheck)
        arq = fopen("Server_UDP_NOCHECK.txt", "a");
    else
        arq = fopen("Server_UDP.txt", "a");

    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arq, "%d", bytes);
    fprintf(arq, " ");    
    fprintf(arq, "%lld", s->cont);
    fprintf(arq, "\n");

}

void send_messages(struct client *c, int num, int nocheck){
    long long int fim, inicio;
    for (int i = 0; i < MAX_SIZE; i++){
        c->buffer[i] = 'a';
    }
    if (nocheck)
        setsockopt(c->sockfd, SOL_SOCKET, SO_NO_CHECK, &(int){1}, sizeof(int));
    inicio = timestamp();
    printf ("Enviando %d bytes para o servidor via UDP\n", num * MAX_SIZE);
    for (int i = 0; i < num; i++){
        int x = sendto(c->sockfd, c->buffer, sizeof(c->buffer), 0, (struct sockaddr*)&(c->servaddr), sizeof(c->servaddr));
    }
    fim = abs(timestamp() - inicio);
    close(c->sockfd);
    printf ("Bytes enviados para o servidor\n");
    printf ("Tempo total: %lldms\n\n", fim);
    /* printf ("Vazão: %lld bytes\n", MAX_SIZE*num/fim); */

    print_client_data(c, num, fim, nocheck); //Função para imprimir dados do cliente no arquivo "Client_UDP.txt"
}

void create_server(struct server *s){
    s->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
}

void receive_messages(struct server *s, int num, int nocheck){
    int bytes;
    socklen_t len = sizeof(s->cli);
    s->cont = 0;
    struct timeval timeout = {3000/1000, (3000%1000)*1000};
    setsockopt(s->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)); //Timeout necessário para saber quando parar de receber pacotes
    printf ("Recebendo bytes do cliente via UDP, %d bytes são esperados\n", num*MAX_SIZE);
    long long int inicio = timestamp();
    while (1){
        bytes = recvfrom(s->sockfd, s->buffer, sizeof(s->buffer), 0, (struct sockaddr*)&s->cli, &len);
        if (bytes == 0 || bytes == -1)
            break;
        s->cont+=bytes;
        memset(s->buffer, 0, sizeof(s->buffer));
    }
    printf ("%lld bytes recebidos do servidor\n", s->cont);
    printf ("%.2f%% dos bytes foram recebidos\n\n", (float)(s->cont * 100)/(float)(num * MAX_SIZE));

    print_server_data(s, num, nocheck); //Função para imprimir dados do servidor no arquivo "Server_UDP.txt"
}

int main(int argc, char* argv[]){
    int num = 0, cflag = 0, sflag = 0, nocheck = 0, c = 0;
    while ((c = getopt(argc, argv, "s:c:d")) != -1)
        switch(c){
            case 'c': //Client
                if (sflag || cflag){
                    printf ("Já existe uma flag ativa\n");
                    exit(1);
                }
                cflag = 1;
                num = atoi(optarg);
                break;
            case 's': //Server
                if (sflag || cflag){
                    printf ("Já existe uma flag ativa\n");
                    exit(1);
                }
                num = atoi(optarg);
                sflag = 1;
                break;
            case 'd': //Disable checksum
                nocheck = 1;
                break;
            default:
                printf ("Uso: ./UDP -c <num de pacotes> ou ./UDP -s <num de pacotes>\n");
                exit(1);
        }
    if (cflag){
        struct client c;
        create_client(&c);
        send_messages(&c, num, nocheck);
    }
    else{
        struct server s;
        create_server(&s);
        receive_messages(&s, num, nocheck);
    }
}