#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BUFFER_SIZE 1024
#define MAX 10000000LL

int main(int argc, char* argv[]){
	int i, tmp, len;
    struct timespec before, after;
    struct sockaddr_in addr;
    char input[BUFFER_SIZE], buf[BUFFER_SIZE];
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0){
        perror("socket create error");
        exit(1);
    }
    if(connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        perror("socket connect error");
        return 1;
    }
    
    for(i = 0; i < BUFFER_SIZE; i++) input[i] = 'a';
    
    clock_gettime(CLOCK_REALTIME, &before);
    
    // start transmission
    for(i = 0; i < MAX; i++){
		tmp = 0;
		while(tmp < BUFFER_SIZE){
			len = send(fd, input + tmp, BUFFER_SIZE - tmp, 0);
			if(len < 0){
				perror("socket send error");
				return 1;
			}
			tmp += len;
		}
        tmp = 0;
    }
    
    input[0] = 'E';
    if(send(fd, input, 1, 0) < 0){
		perror("receive error");
		exit(1);
	}
	
    if(recv(fd, buf, BUFFER_SIZE, 0) < 0){
		perror("receive error");
		exit(1);
	}
    clock_gettime(CLOCK_REALTIME, &after);
    
    long bytes = MAX * BUFFER_SIZE + 1;
    int interval = after.tv_sec - before.tv_sec;
    printf("%ld %d %.6f", bytes, interval, (float)(bytes * 8 /1000000.0)/interval);
    close(fd);
    return 0;
}
