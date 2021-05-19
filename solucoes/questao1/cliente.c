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

#define PORT 8000
#define BUFFMAX 256    //tamanho maximo da string

int main (int argc, char **argv) {

    struct sockaddr_in endCli, endServ;

    int sock;

    char buffer_in[BUFFMAX];
    char buffer_out[BUFFMAX];
    int res, length;

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

    bzero ((char *)&endCli, sizeof (endCli));
    endCli.sin_family = AF_INET;
    endCli.sin_port = htons(0);
    endCli.sin_addr.s_addr = htonl(INADDR_ANY); 

    bzero ((char *)&endServ, sizeof (endServ));
    endServ.sin_family = AF_INET;
    endServ.sin_port = htons(PORT);
    endServ.sin_addr.s_addr = inet_addr(argv[1]);

    bind(sock, (struct sockaddr*) &endCli, sizeof(endCli));

    printf("IP: %s\n", argv[1]);

    while(1){
        bzero ((char *)&buffer_in, sizeof (buffer_in));
        bzero ((char *)&buffer_out, sizeof (buffer_out));
        printf ("\nMensagem ('exit' to quit): ");
       
        scanf("%s", &buffer_out);

        res = sendto(sock, buffer_out, strlen(buffer_out), 0, (struct sockaddr *) &endServ, (socklen_t) sizeof(endServ));
        pause(2);

        if (res == -1) {
            perror("Sendto");
            exit (0);
        }

        length = sizeof(endServ);

        res = recvfrom(sock, buffer_in, BUFFMAX, 0, (struct sockaddr *) &endServ, (socklen_t*)&length);

        if (res == -1) {
            perror("Recvfrom");
            exit (0);
        }

        if(!strcmp(buffer_in, "exit")){
            break;
        }

        printf ("\nResposta: %s", buffer_in);
    }

    return 0;
}
