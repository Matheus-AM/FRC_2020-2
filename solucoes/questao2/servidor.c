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

#define PORT 5000
#define ERROR -1
#define BUFFMAX 256     //tamanho maximo da string
#define BACKLOG 5

int main (int argc, char *argv[]) {

    if(argc<3)  {
        printf("uso correto: %s <ip_do_servidor> <porta_do_servidor>\n", argv[0]);
        exit(1);  
    }

    int sock;
    char buffer_in[BUFFMAX];
    char buffer_out[BUFFMAX];
    int res;

    if (argc < 2) {
        printf ("Use %s <host>\n\n", argv [0]);
        exit (0);
    }

    sock = socket (AF_INET, SOCK_STREAM, 0);
    //domain –  AF_INET for IPv4/ AF_INET6 for IPv6 
    //type – SOCK_STREAM for TCP / SOCK_DGRAM for UDP 
    //0 means use default protocol for the address family.

    if (sock == ERROR) {
        perror ("Socket");
        exit (0);
    }

    struct sockaddr_in endServ, endCli;

    bzero ((char *)&endCli, sizeof (endCli));
    endCli.sin_family = AF_INET;
    endCli.sin_port = htons(0);
    endCli.sin_addr.s_addr = htonl(INADDR_ANY); 

    bzero ((char *)&endServ, sizeof (endServ));
    endServ.sin_family = AF_INET;
    endServ.sin_port = htons(PORT);
    endServ.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(sock, (struct sockaddr*) &endServ, sizeof(endServ)) == -1) {
        /*
        socket file descriptor
        address
        address.length
        */
        perror("Bind");
        exit(0);
    }
    int connections = 0;
    struct sockaddr_in newNetworks[BACKLOG];

    listen(sock, BACKLOG);
    /*
    socket file descriptor
    backlog: maximum lenght queue of connections
    */

    while(connections <= BACKLOG){

        int cliLen = sizeof(endCli)
        int clientSock = accept(sock, (struct sockaddr *) &endCli, &cliLen);
        

        if (clientSock == ERROR) {
            perror ("Connect");
            exit (0);
        }

        // getting the address port and remote host
        printf("Local Port: %d\n", PORT);
        printf("Remote Host: %s\n", inet_ntoa(network.sin_addr));

        while(1){
            printf ("\nCliente: ");
            scanf("%s", buffer_out);
            if (!strcmp (buffer_out, "exit")) {
                break;
            }
            res = send(sock, buffer_out, sizeof(buffer_out), 0);
            /* 
            socket file descriptor
            mensage (buffer)
            mensage.length (length)
            flag (0 default)
            */
            if (res == ERROR) {
                perror ("Send");
                exit (0);
            }


            res = recv(clientSock, buffer_in, sizeof(buffer_in), 0);
            /* 
            socket file descriptor
            mensage (buffer)
            mensage.length (length)
            flag (0 default)
            */
            if (res == ERROR) {
                perror ("Recive");
                exit (0);
            }
            printf("\nServidor: %s", buffer_in);

        }
        close(clientSock)
    }
    close(sock);
    return 0;
}
