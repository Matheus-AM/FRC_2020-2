#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORT 53
#define BUFFMAX 256    //tamanho maximo da string

int main (int argc, char * * argv) {

    struct sockaddr_in network;

    int sock;

    char buffer_in[BUFFMAX];
    char buffer_out[BUFFMAX];
    int request, response, length;

    if (argc < 2) {
        printf ("Use %s <host>\n\n", argv [0]);
        exit (0);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    //domain –  AF_INET for IPv4/ AF_INET6 for IPv6 
    //type – SOCK_STREAM for TCP / SOCK_DGRAM for UDP 
    //0 means use default protocol for the address family.

    if (sock == -1) {
        perror("Socket");
        exit(0);
    }

    bzero ((char *)&network, sizeof (network));
    network.sin_family = AF_INET;
    network.sin_port = htons(PORTA);
    network.sin_addr.s_addr = htonl(INADDR_ANY); 


    if(bind(sock, (struct sockaddr*) &network, sizeof(network)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    printf("IP: %s\n", INADDR_ANY);

    while(1){
        response = recvfrom(sock, buffer_in, BUFFMAX, 0, (struct sockaddr *) &network, (socklen_t*)&length);

        if (response == -1) {
            perror("Recvfrom");
            exit (0);
        }

        if(!strcmp(buffer_in, "exit")){
            break;
        }
        printf ("\nResposta: %s", buffer_in);


        printf ("\nMensagem: ");

        scanf("%s", &buffer_out);

        request = sendto(sock, buffer_out, BUFFMAX, 0, (struct sockaddr *) &network, (socklen_t) sizeof(network));

        if (request == -1) {
            perror("Sendto");
            exit (0);
        }

    }

    close(sock);

    return 0;
}
