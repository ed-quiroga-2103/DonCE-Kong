#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <memory.h>
#include <zconf.h>

#define MAX 80
#define PORT 8888
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int sockfd)
{
    printf("\n\n\n\n");
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("\nChoose a barrel: Type 1 runs down the platforms\nand Type 2 flies down vertically\nType 3 is a mix of type 1 and type 2\n");
        printf("\nType ':exit' to shut down the server. THIS WILL MAKE THE GAME CRASH IF YOU DONT CLOSE IT BEFORE.\n\n");
        printf("Barrel type: ");
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        if((strncmp(buff, "2", 1)) == 0){

            printf("1: Left Side, 2: Middle, 3: Right Side\n");
            n--;
            while ((buff[n++] = getchar()) != '\n')
                ;


        }

        if((strncmp(buff, "3", 1)) == 0){

            printf("1: Left Side, 2: Middle, 3: Right Side\n");
            n--;
            while ((buff[n++] = getchar()) != '\n')
                ;


        }

        else if((strncmp(buff,":exit",5)) == 0){
            printf("Server Exit...\n");
            break;
        }

        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));
    }
}

// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    close(sockfd);
}