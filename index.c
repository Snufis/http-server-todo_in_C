#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main(){
    struct sockaddr_in addr = {
        .sin_family      = AF_INET,
        .sin_port        = htons(8080),
        .sin_addr.s_addr = INADDR_ANY
    };

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        return 1;
    }

    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("bind");
        return 1;
    }

    if(listen(fd, 10) == -1){ 
        perror("listen");
        return 1;
    }

    while(1) {
        int client_fd = accept(fd, NULL,NULL);
        if(client_fd == -1){
            perror("accept");
            return 1;
        }
    
        char buf[1028] = {0};
        if(recv(client_fd, buf, sizeof(buf), 0) == -1){
            perror("recv");
            return 1;
        }
        
        char method[8],path[64];
        sscanf(buf, "%s %s", method, path);
        
        char *response;

        if (strcmp(path, "/") == 0) {
            response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nStrona glowna";
        }       else if (strcmp(path, "/add") == 0) {
            response = "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\nDodaj todo";
        }       
        else {
            response = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot found";
        }       

        
        
        if(send(client_fd, response, strlen(response), 0) == -1){
            perror("send");
        }   

        if(close(client_fd) == -1){
            perror("close");
        }
    }

 

    printf("jest git");
}