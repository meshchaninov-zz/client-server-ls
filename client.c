#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "form.h"

#define BUF_SIZE 500

int main(int argc, char* argv[]) {
    if(argc != 5) {
        fprintf(stderr, "Wrong number of arguments\n");
        fprintf(stderr, "Write:\n %s <path> <expancion> <ip> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("Failed to create socket");
        close(sock);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[4]);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(argv[3]);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }
    puts("Connection success");
    
    struct form f;
    strncpy(f.path, argv[1], sizeof(f.path));
    strncpy(f.e,argv[2],sizeof(f.e));
   
    char buf[BUF_SIZE];
    puts("Send..");
    fprintf(stdout, "\nPath: %s\n", f.path);
    fprintf(stdout, "Extencion: %s\n\n", f.e);

    if(send(sock, (const void*)&f, sizeof(f),0) < 0) {
        perror("Sending failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if(recv(sock, (void*)buf, sizeof(buf),0) < 0) {
        perror("Recv failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    puts("Result: ");
    puts(buf);
    
    close(sock);
    exit(EXIT_SUCCESS);
}
