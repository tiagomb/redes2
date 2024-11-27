#ifndef ___UTILS___
#define ___UTILS___

#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_SIZE 10000
#define SERVER_ADDRESS "192.168.1.2"

struct client {
    struct sockaddr_in servaddr;
    int sockfd;
    char buffer[MAX_SIZE];
};

struct server {
    struct sockaddr_in servaddr, cli;
    int sockfd;
    int connfd;
    long long int cont;
    char buffer[MAX_SIZE];
};

//Função utilizada para medir os tempos de envio dos pacotes
long long int timestamp();

//Função que cria o socket do cliente, configurando-o para enviar mensagens para o IP definido em SERVER_ADDRESS na porta 8080
void create_client(struct client *c);

//Função que imprime os valores necessários para gerar o gráfico do cliente no formato correto em um arquivo (num_bytes tempo)
//O parâmetro nocheck é usado somente para UDP
void print_client_data(struct client *c, int num, long long int fim, int nocheck);

//Função que imprime os valores necessários para gerar o gráfico do servidor no formato correto em um arquivo (bytes_total bytes_rec)
//O parâmetro nocheck é usado somente para UDP
void print_server_data(struct server *s, int num, int nocheck);

//Função que envia num pacotes de tamanho MAX_SIZE para o servidor. Imprime na saída padrão quantos bytes foram enviados, quando a transmissão terminou e quanto tempo levou
//O parâmetro nocheck é usado somente para UDP
void send_messages(struct client *c, int num, int nocheck);

//Função que cria o socket do servidor, configurando-o para escutar mensagens de qualquer IP na porta 8080
void create_server(struct server *s);

//Função que recebe os pacotes enviados pelo cliente. Imprime na saída padrão a quantidade de bytes esperados, de bytes recebidos e a porcentagem recebida
//O parâmetro nocheck é usado somente para UDP
void receive_messages(struct server *s, int num, int nocheck);

#endif // ___UTILS___