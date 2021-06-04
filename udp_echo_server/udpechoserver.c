#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 16

int main(int argc, char* argv[]){
    int len, sent, done;
    char buf[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t clen = sizeof(client_addr);
    int server_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    if(server_fd < 0){
        perror("socket create error");
        exit(1);
    }
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("socket bind error");
        exit(1);
    }

    while((len = recvfrom(server_fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &clen)) > 0){
		sent = 0;
		while(sent < len){
			done = sendto(server_fd, buf + sent, len - sent, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
		    if(done < 0){
			    perror("socket send error");
			    return 1;
		    }
		    sent += done;
		}
	}
	close(server_fd);
	if(len < 0){
        perror("receive error");
        return 1;
    }
}
