#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include "form.h"

#define BUF_SIZE 100

int sock, new_socket;
void handler() {
    close(sock);
    shutdown(new_socket, 2);
    printf("\nBye-Bye!\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        fprintf(stderr, "Write:\n %s <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handler);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }
    puts("Socket created");
    
    int port = atoi(argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind socket failed");
        exit(EXIT_FAILURE);
    }
    puts("Bind done");
    
    if(listen(sock, 5) < 0) {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }
    puts("Waiting for incoming connections...");
    
    size_t sock_size;
    
    int fd[2];
    pipe(fd);
    for(;;) {
        struct sockaddr_in addr_a;
        
        sock_size = sizeof(addr_a);
        if((new_socket = accept(sock, (struct sockaddr*)&addr_a, (socklen_t*)&sock_size)) < 0) {
            perror("Failed to accept");
            exit(EXIT_FAILURE);
        }
        puts("Connections accepted");

        struct form f;
        if(recv(new_socket, (void *)&f, sizeof(f), 0) < 0) {
            perror("Failed to recv");
            close(new_socket);
            exit(EXIT_FAILURE);
        }
        
        fprintf(stdout, "Path: %s\n", f.path);
        fprintf(stdout, "Extencion: %s\n", f.e);
        
        if(fork() == 0) {
           puts("doing find..");
           char enter = '\n';
            write(fd[1], &enter, sizeof(enter));
            close(fd[0]);
            close(1);
            close(2);
            fcntl(fd[1], F_DUPFD, 1);
            fcntl(fd[1], F_DUPFD, 2);
            char str[PATH_SIZE+EX_SIZE+2];
            strcpy(str, "*.");
            strcat(str,f.e);
            execl("/usr/bin/find", "find", f.path, "-name", str,NULL);
            perror("ls");
            exit(EXIT_FAILURE);
        } 
        wait(NULL);
        
        puts("read data");
        char buff[BUF_SIZE];
        memset((void *)buff, 0, sizeof(buff));
        int r = read(fd[0], buff, sizeof(buff));
        if( r == 1) strcpy(buff, "Not found"); 
        puts(buff);
        
        puts("Send data"); 
        if(send(new_socket, (const void*)buff, sizeof(buff), 0) < 0) {
             perror("Failed to send");
             close(new_socket);
             exit(EXIT_FAILURE);
        }
        puts("Done");
        close(new_socket);
        puts("Connection close");
   } 
   
   exit(EXIT_SUCCESS);
}
