#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string.h>

void create_client(struct client *c){
    c->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (c->sockfd == -1){
        printf("Falha ao criar socket\n");
        exit(1);
    }
    c->servaddr.sin_family = AF_INET;
    inet_aton(SERVER_ADDRESS, &(c->servaddr.sin_addr));
    c->servaddr.sin_port = htons(8080);
    if (connect(c->sockfd, (struct sockaddr*)&(c->servaddr), sizeof(c->servaddr)) != 0){
        printf ("Conexão falhou");
        exit(1);
    }
}

void print_client_data(struct client *c, int num, long long int fim, int nocheck){
    int bytes = num * MAX_SIZE;
    char space = ' ';
    FILE *arq = NULL;

    arq = fopen("Client_TCP.txt", "a");

    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arq, "%d %lld\n", bytes, fim);

    fclose(arq);
}

void print_server_data(struct server *s, int num, int nocheck){
    int bytes = num * MAX_SIZE;
    char space = ' ';
    FILE *arq = NULL;

    arq = fopen("Server_TCP.txt", "a");

    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arq, "%d %.2f\n", bytes, (double)s->cont *100 / bytes);

    fclose(arq);
}

void send_messages(struct client *c, int num, int nocheck){
    long long int fim, inicio;
    int bytes;
    for (int i = 0; i < MAX_SIZE; i++){
        c->buffer[i] = 'a';
    }
    inicio = timestamp();
    printf ("Enviando %d bytes para o servidor via TCP\n", num*MAX_SIZE);
    for (int i = 0; i < num; i++){
        bytes = write(c->sockfd, c->buffer, sizeof(c->buffer));
    }
    fim = abs(timestamp() - inicio);
    printf ("Bytes enviados para o servidor\n");
    printf ("Tempo total: %lldms\n\n", fim);
    /* printf ("Vazão: %lld bytes\n", (MAX_SIZE*num)/fim); */
    
    print_client_data(c, num, fim, nocheck); //Função para imprimir dados do cliente no arquivo "Client_TCP.txt"

    close(c->sockfd);
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
    setsockopt(s->sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)); //Necessário para permitir várias execuções consecutivas na mesma porta
    if (bind(s->sockfd, (struct sockaddr*)&(s->servaddr), sizeof(s->servaddr)) != 0){
        printf ("Bind falhou\n");
        exit(1);
    }
    if ((listen(s->sockfd, 2) != 0)){
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

void receive_messages(struct server *s, int num, int nocheck){
    int bytes;
    struct timeval timeout = {3000/1000, (3000%1000)*1000};
    s->cont = 0;
    setsockopt(s->connfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    printf ("Recebendo bytes do cliente via TCP, %d bytes são esperados\n", num*MAX_SIZE);
    while (1){
        bytes = read(s->connfd, s->buffer, sizeof(s->buffer));
        if (bytes == 0)
            break;
        s->cont+=bytes;
        memset(s->buffer, 0, sizeof(s->buffer));
    }
    printf ("%lld bytes recebidos do cliente\n", s->cont);
    printf ("%.2f%% dos bytes foram recebidos\n\n", (float)(s->cont*100)/(float)(num * MAX_SIZE));
    
    print_server_data(s, num, nocheck); //Função para imprimir dados do server no arquivo "Server_TCP.txt"
    
    close(s->connfd);
    close(s->sockfd);
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
        send_messages(&c, num, 0);
    }
    else{
        struct server s;
        create_server(&s);
        receive_messages(&s, num, 0);
    }
}
