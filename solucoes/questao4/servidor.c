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
    struct sockaddr_in newcli[BACKLOG];

    listen(sock, BACKLOG);
    /*
    socket file descriptor
    backlog: maximum lenght queue of connections
    */

    int cliLen = sizeof(endCli);
    int clientSock = accept(sock, (struct sockaddr *) &endCli, &cliLen);
    

    if (clientSock == ERROR) {
        perror ("Connect");
        exit (0);
    }
    pid_t cpid = fork();

    if(cpid == 0) {
        while(1){
            bzero ((char *)&buffer_in, sizeof (buffer_in));
            
            res = recv(clientSock, buffer_in, sizeof(buffer_in), 0);
            if (!strcmp (buffer_in, "exit")) {
                break;
            }
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
            printf("\nCliente: %s", buffer_in);
        }
        exit(0);
    }
    else{
        while(1){
            bzero ((char *)&buffer_out, sizeof (buffer_out));
            
            printf ("\nServidor: ");
            scanf(" %s", buffer_out);

            res = send(clientSock, buffer_out, sizeof(buffer_out), 0);
            if(!strcmp(buffer_out, "exit")){
                break;
            }
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
        }
    }
    close(clientSock);
    close(sock);
    return 0;
}
