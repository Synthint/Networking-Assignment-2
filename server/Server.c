// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

void removeCharacter(char s[], char c)
{
    size_t j = 0;
    for (size_t i = 0; s[i] != '\0'; ++i) {
        if (s[i] != c) {
            s[j] = s[i];
            ++j;
        }
    }
    s[j] = '\0';
}

struct User{
    char username[50];
    char password[50];
    char name[50];
    char major[50];
    char advisor[50];
};

int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };


    FILE *fp = fopen("passwd.txt", "r");
    if(fp == NULL){
        perror("Reading file failed");
        exit(EXIT_FAILURE);
    }

    char in;
    int userNumber = 0;
    int itemNumber = 0;

    struct User allUsers[5];

    char tempString[50] = "";
    while ((in = fgetc(fp)) != EOF){
        //putchar(in);
       if(in == '('){
        strcpy(tempString,"");
        itemNumber = 1;
        continue;
       }else if (in == ',' || in == ')'){
        //printf("%s\n",tempString);
        removeCharacter(tempString,'\n');
        removeCharacter(tempString,'\r');

        switch(itemNumber){
            case 1:
                strcpy(allUsers[userNumber].username,tempString);
                break;
            case 2:
                strcpy(allUsers[userNumber].password,tempString);
                break;
            case 3:
                strcpy(allUsers[userNumber].name,tempString);
                break;
            case 4:
                strcpy(allUsers[userNumber].major,tempString);
                break;
            case 5:
                strcpy(allUsers[userNumber].advisor,tempString);
                break;
        }
        strcpy(tempString,"");
        itemNumber++;
        continue;
       }else if (in == ';'){
        itemNumber = 0;
        userNumber++;
        continue;
       }else{
        strncat(tempString, &in, 1);
       }
       
    }

    fclose(fp);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    send(new_socket, "Welcome to Computer Science Department", strlen("Welcome to Computer Science Department"), 0);

    int attempts = 0;
    int loginSuccessful = 0;
    int validUser = -1;
    while(attempts < 3){
        bzero(buffer,256);
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        
        for(int x=0;x<5;x++){
            //printf("==========\n");
            //printf("%s -> %d\n",allUsers[x].username,strlen(allUsers[x].username));
            //printf("%s -> %d\n",buffer,strlen(buffer));
            //printf("==========\n");

            if(strcmp(allUsers[x].username,buffer) == 0){
                validUser = x;
                printf("Valid User\n");
                break;
            }
        }
        bzero(buffer,256);
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        if(validUser>=-1 && strcmp(allUsers[validUser].password,buffer) == 0){
                loginSuccessful = 1;
                printf("Valid Pass\n");
                break;
            }
        if(attempts < 2){
            send(new_socket, "Incorrect", strlen("Incorrect"), 0);
        }
        attempts++;
    }
    if(loginSuccessful == 0){
        send(new_socket, "Failed Login", strlen("Login Failed"), 0);
        send(new_socket, "Only For ESU CPSC Students Taking CPSC445\nYou Are Not Invited Yet\n", strlen("Only For ESU CPSC Students Taking CPSC445\nYou Are Not Invited Yet\n"), 0);
        
    }else{
        send(new_socket, "Login Successful", strlen("Login Successful"), 0);
        valread = read(new_socket, buffer, 1024);
        send(new_socket, "Welcome to CPSC445 - Computer Networking Class\nYou Are Invited to Use Your Own Machine\n", strlen("Welcome to CPSC445 - Computer Networking Class\nYou Are Invited to Use Your Own Machine\n"), 0);
        valread = read(new_socket, buffer, 1024);
        send(new_socket, allUsers[validUser].name,strlen(allUsers[validUser].name),0);
        valread = read(new_socket, buffer, 1024);
        send(new_socket, allUsers[validUser].major,strlen(allUsers[validUser].major),0);
        valread = read(new_socket, buffer, 1024);
        send(new_socket, allUsers[validUser].advisor,strlen(allUsers[validUser].advisor),0);

    }
    
    printf("\n\n");
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}