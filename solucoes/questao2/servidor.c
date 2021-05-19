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
    int connection, sended, recived;

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

    struct sockaddr_in network;
    bzero ((char *) &network, sizeof(network));
    network.sin_family = AF_INET;
    network.sin_port = htons(PORT);
    network.sin_addr.s_addr = INADDR_ANY; 

    if(bind(sock, (struct sockaddr*) &network, sizeof(network)) == -1) {
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


        int clientSock = accept(sock, NULL, NULL);
        

        if (connection == ERROR) {
            perror ("Connect");
            exit (0);
        }

        // getting the address port and remote host
        printf("Local Port: %d\n", PORT);
        printf("Remote Host: %s\n", inet_ntoa(network.sin_addr));

        while(1){
            printf ("\nCliente: ");
            scanf("%s", buffer_out);
            sended = send(sock, buffer_out, sizeof(buffer_out), 0);
            /* 
            socket file descriptor
            mensage (buffer)
            mensage.length (length)
            flag (0 default)
            */
            if (sended == ERROR) {
                perror ("Send");
                exit (0);
            }
            if (!strcmp (buffer_out, "exit")) {
                break;
            }

            recived = recv(sock, buffer_in, sizeof(buffer_in), 0);
            /* 
            socket file descriptor
            mensage (buffer)
            mensage.length (length)
            flag (0 default)
            */
            if (recived == ERROR) {
                perror ("Recive");
                exit (0);
            }
            printf("\nServidor: %s", buffer_in);

        }
    }
    close(sock);
    return 0;
}
