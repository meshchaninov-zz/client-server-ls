#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "form.h"

#define PORT 2011
#define BUF_SIZE 500

int main(int argc, char* argv[]) {
    if(argc != 4) {
        fprintf(stderr, "Wrong number of arguments\n");
        fprintf(stderr, "Write <path> <expancion> <ip>\n");
        exit(EXIT_FAILURE);
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(argv[3]);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }
    puts("Connection success");
    
    struct form f;
    strncpy(f.path, argv[1], sizeof(f.path));
    strncpy(f.e,argv[2],sizeof(f.e));
   
    char buf[BUF_SIZE];
    puts("Send..");
    puts(f.path);
    puts(f.e);
    if(send(sock, (const void*)&f, sizeof(f),0) < 0) {
        perror("send");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if(recv(sock, (void*)buf, sizeof(buf),0) < 0) {
        perror("recv");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    puts(buf);
    
    close(sock);
    exit(EXIT_SUCCESS);
}
