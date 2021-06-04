#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 16

int main(int argc, char* argv[]){
  int k, len, cnt, get, tmp, written, done;
  struct sockaddr_in addr;
  socklen_t slen = sizeof(addr);
  char input[BUFFER_SIZE], buf[BUFFER_SIZE];
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(fd < 0){
    perror("socket create error");
    exit(1);
  }

  while((k = read(0, input, sizeof(input))) > 0){
	  cnt = 0;
	  while(cnt < k){
		  len = sendto(fd, input + cnt, k - cnt, 0, (struct sockaddr *)&addr, sizeof(addr));
		  if(len < 0){
			  perror("socket send error");
			  return 1;
		  }
		  cnt += len;
		  get = 0;
          while(get < len){
			  tmp = recvfrom(fd, buf, BUFFER_SIZE, 0, (struct sockaddr *)&addr, &slen);
			  if(tmp < 0){
				  perror("receive error");
				  return 1;
			  }
			  if(tmp == 0) break;
			  get += tmp;
			  written = 0;
			  while(written < tmp){
				  done = write(1, buf + written, tmp - written);
				  if(done < 0){
					  perror("write error");
					  return 1;
				  }
				  written += done;
			  }
		  }
      }
  }
  close(fd);
  if(k < 0){
    perror("read error");
    return 1;
  }
  return 0;
}
