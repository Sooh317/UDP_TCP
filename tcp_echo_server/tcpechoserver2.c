#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_EVENTS 5
#define SIZE 10
#define BUFFER_SIZE 8

int main(int argc, char* argv[]){
  int i, nfds, client_fd, fd, len, tmp, done;
  char buf[BUFFER_SIZE];
  struct sockaddr_in client_addr, server_addr;
  socklen_t clen = sizeof(client_addr);
  struct epoll_event ev, events[MAX_EVENTS];
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[1]));
  server_addr.sin_addr.s_addr = INADDR_ANY;
  int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  int epoll_fd = epoll_create1(0);

  if(epoll_fd < 0){
    perror("epoll create error");
    exit(1);
  }
  if(server_fd < 0){
    perror("socket create error");
    exit(1);
  }
  if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
    perror("socket bind error");
    exit(1);
  }
  if(listen(server_fd, SIZE) == -1){
    perror("socket listen error");
    exit(1);
  }

  ev.data.fd = server_fd;
  ev.events = EPOLLIN | EPOLLOUT;

  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0){
    perror("epoll control error");
    return 1;
  }
  while(1){
    nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if(nfds < 0){
      perror("epoll wait error");
      return 1;
    }
    for(i = 0; i < nfds; i++){
      if(events[i].data.fd == server_fd){
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &clen);
        if(client_fd < 0){
          perror("accept");
          return 1;
        }
        
        ev.events = EPOLLIN | EPOLLOUT;
        ev.data.fd = client_fd;
        
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0){
          perror("epoll control error");
          return 1;
        }
      }
      else{
        fd = events[i].data.fd;
        len = recv(fd, buf, BUFFER_SIZE, 0);
        buf[len] = '\0';
        if(len == 0){
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
          //close(fd);
        }
        if(len < 0){
          perror("receive error");
          return 1;
        } 
        tmp = 0;
        while(tmp < len){
			done = send(fd, buf + tmp, len, 0);
			if(done < 0){
				perror("send error");
				exit(1);
			}
			tmp += done;
        }
      }
    }
  }
}
