#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]){
	struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    int client_fd, server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int len;
    socklen_t clen = sizeof(client_addr);
    char buf[BUFFER_SIZE];
    
    if(server_fd < 0){
      perror("socket create error");
      return 1;
    }
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
      perror("socket bind error");
      return 1;
    }
    if(listen(server_fd, 1) == -1){
      perror("socket listen error");
      return 1;
    }
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &clen);
    if(client_fd == -1){
      perror("socket accept error");
      return 1;
    }
    
    while((len = recv(client_fd, buf, BUFFER_SIZE, 0)) > 0){
		if(buf[len - 1] == 'E'){
			if(send(client_fd, buf, 1, 0) < 0){
				perror("send error");
				return 1;
			}
		}
	}
	if(len < 0){
		perror("receive error");
		return 1;
	}
    close(client_fd);
    close(server_fd);
}
