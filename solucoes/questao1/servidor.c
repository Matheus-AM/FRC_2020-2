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

int main (int argc, char * * argv) {

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

    if(bind(sock, (struct sockaddr*) &endServ, sizeof(endServ)) == -1) {
        perror("bind failed");
        exit(0);
    }

    while(1){
        bzero ((char *)&buffer_in, sizeof (buffer_in));
        bzero ((char *)&buffer_out, sizeof (buffer_out));

        length = sizeof(endCli);
        res = recvfrom(sock, buffer_in, BUFFMAX, 0, (struct sockaddr *) &endCli, (socklen_t*)&length);
        if(!strcmp(buffer_in, "exit")){
            break;
        }


        if (res == -1) {
            perror("Recvfrom");
            exit (0);
        }


        printf ("\nResposta: %s", buffer_in);


        printf ("\nMensagem ('exit' to quit): ");

        scanf(" %s", &buffer_out);

        res = sendto(sock, buffer_out, BUFFMAX, 0, (struct sockaddr *) &endCli, (socklen_t) sizeof(endCli));
        if(!strcmp(buffer_out, "exit")){
            break;
        }
        
        if (res == -1) {
            perror("Sendto");
            exit (0);
        }

    }

    close(sock);

    return 0;
}
