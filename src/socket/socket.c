#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "communication.h"

#ifndef __PORT__
#define __PORT__ 4080
#endif

typedef struct s_thread_arg
{
  pthread_t thread_id;
  int socket;
} thread_arg_t;

void *socket_read(void *arg);

static int server_fd;
static struct sockaddr_in address;

void socket_init()
{
  int opt = 1;
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(__PORT__);
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}

void socket_accept()
{
  int new_socket;
  thread_arg_t *thread_arg;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  while (1)
  {
    pthread_t thread_id;

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
      continue;
    }

    thread_arg = (thread_arg_t *)malloc(sizeof(thread_arg_t));
    thread_arg->thread_id = thread_id;
    thread_arg->socket = new_socket;
    pthread_create(&thread_id, NULL, socket_read, thread_arg);
  }
}

void *socket_read(void *arg)
{
  int numread;
  player_message_t request;
  thread_arg_t *thread_arg = (thread_arg_t *)arg;

  while (1)
  {
    memset(&request, 0, sizeof(player_message_t));
    numread = read(thread_arg->socket, &request, sizeof(player_message_t));

    if (numread == 0)
    {
      break;
    }

    request.socket = thread_arg->socket;
    callback_list_call(request.type, &request);
  }

  free(thread_arg);

  return NULL;
}

void socket_send(int socket, player_message_t *message)
{
  send(socket, message, sizeof(player_message_t), 0);
}
