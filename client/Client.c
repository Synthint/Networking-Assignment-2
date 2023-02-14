// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
 
int main(int argc, char const* argv[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    char bufferIn[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "192.168.0.5", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if ((client_fd
         = connect(sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    bzero(buffer,256);
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

    bzero(bufferIn,256);
    printf("\n\nEnter Username:");
    fgets(bufferIn,256,stdin);
    bufferIn[strlen(bufferIn)-1] = '\0';
    send(sock, bufferIn, strlen(bufferIn), 0);

    bzero(bufferIn,256);
    printf("Enter Password:");
    fgets(bufferIn,256,stdin);
    bufferIn[strlen(bufferIn)-1] = '\0';
    send(sock, bufferIn, strlen(bufferIn), 0);

    bzero(buffer,256);
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

    while((buffer[0] == 'I')){
        bzero(bufferIn,256);
        printf("\n\nEnter Username:");
        fgets(bufferIn,256,stdin);
        bufferIn[strlen(bufferIn)-1] = '\0';
        send(sock, bufferIn, strlen(bufferIn), 0);

        bzero(bufferIn,256);
        printf("Enter Password:");
        fgets(bufferIn,256,stdin);
        bufferIn[strlen(bufferIn)-1] = '\0';
        send(sock, bufferIn, strlen(bufferIn), 0);

        bzero(buffer,256);
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
    }

    if(buffer[0] == 'F'){
        bzero(buffer,256);
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);


    }else{

        send(sock, "R", strlen("R"), 0);
        bzero(buffer,256);
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
        
        send(sock, "R", strlen("R"), 0);
        bzero(buffer,256);
        valread = read(sock, buffer, 1024);
        printf("Hello %s\n", buffer);

        send(sock, "R", strlen("R"), 0);
        bzero(buffer,256);
        valread = read(sock, buffer, 1024);
        printf("Your Major is: %s\n", buffer);

        send(sock, "R", strlen("R"), 0);
        bzero(buffer,256);
        valread = read(sock, buffer, 1024);
        printf("Your Advisor is: %s\n", buffer);

    }
    
    printf("\n\n", buffer);
 
    // closing the connected socket
    close(client_fd);
    return 0;
}