#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 8

int main(int argc, char* argv[]){
  int k, cnt, tmp, len, get;
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
  
  int tot = 0;
  
  while((k = read(0, input, BUFFER_SIZE)) > 0){
    cnt = 0;
    while(cnt < k){
      tmp = 0;
      len = send(fd, input + cnt, k - cnt, 0);
      if(len < 0){
        perror("socket send error");
        return 1;
      }
      cnt += len;
      while(tmp < len){
        get = recv(fd, buf, BUFFER_SIZE, 0);
        tot += get;
        if(get < 0) perror("receive error");
        tmp += get;
        buf[get] = '\0';
        int written = 0;
        while(written < get){
          int done = write(1, buf + written, get - written);
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
  fprintf(stderr, "received charactres from server = %d\n", tot); // total number of charactres received from server. (for debug)
  return 0;
}
