#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#define THREAD_SIZE 10
#define BUFFER_SIZE 8

void* thread_func(void* arg);
void init(char* argv[]);

int main(int argc, char* argv[]){
  init(argv);
}

void init(char* argv[]){
  pthread_t threads[THREAD_SIZE];
  int i;
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[1]));
  server_addr.sin_addr.s_addr = INADDR_ANY;
  int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(server_fd < 0){
    perror("socket create error");
    exit(1);
  }
  if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
    perror("socket bind error");
    exit(1);
  }
  if(listen(server_fd, THREAD_SIZE + 1) == -1){
    perror("socket listen error");
    exit(1);
  }
  for(i = 0; i < THREAD_SIZE; i++){
    if(pthread_create(&threads[i], NULL, thread_func, &server_fd) != 0){
      perror("pthread create failed\n");
      exit(1);
    }
  }
  for(i = 0; i < THREAD_SIZE; i++){
    if(pthread_join(threads[i], NULL) != 0){
      perror("pthread join failed\n");
      exit(1);
    }
  }
  close(server_fd);
}
void* thread_func(void* arg){
  int server_fd = *(int *)arg;
  struct sockaddr_in client_addr;
  socklen_t clen = sizeof(client_addr);
  int client_fd;
  char buf[BUFFER_SIZE];
  int len, get, tmp;
  while(1){
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &clen);
    if(client_fd == -1){
      perror("socket accept error");
      exit(1);
    }
    while((len = recv(client_fd, buf, BUFFER_SIZE, 0)) > 0){
      //buf[len] = '\0';
      //printf("message for client_fd %d is %s\n",client_fd,buf); // for debug
      if(len < 0){
        perror("receive error");
        exit(1);
      }
      get = 0;
      while(get < len){
        tmp = send(client_fd, buf + get, len - get, 0);
        if(tmp < 0){
          perror("write error");
          exit(1);
        }
        get += tmp;
      }
    }
    close(client_fd);
  }
  pthread_exit(NULL);
}
