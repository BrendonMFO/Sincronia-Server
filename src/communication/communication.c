#include <stdlib.h>
#include <string.h>
#include "communication.h"

static communication_callback_list_t callback_list;

void callback_list_init()
{
  callback_list.first = NULL;
  callback_list.last = NULL;
}

void callback_list_push(int type, void (*callback)(player_message_t *, player_message_t *))
{
  communication_callback_t *new_callback = (communication_callback_t *)malloc(sizeof(communication_callback_t));
  if (new_callback == NULL)
  {
    exit(EXIT_FAILURE);
  }
  new_callback->type = type;
  new_callback->callback = callback;

  if (callback_list.first == NULL && callback_list.last == NULL)
  {
    new_callback->prev = NULL;
    new_callback->next = NULL;
    callback_list.first = new_callback;
    callback_list.last = new_callback;
  }
  else
  {
    new_callback->prev = callback_list.last;
    callback_list.last->next = new_callback;
    callback_list.last = new_callback;
  }
}

void callback_list_call(int type, player_message_t *request)
{
  communication_callback_t *callback;
  player_message_t response;
  memset(&response, 0, sizeof(player_message_t));
  for (callback = callback_list.first; callback != NULL; callback = callback->next)
  {
    if (callback->type == type)
    {
      callback->callback(request, &response);
      if (response.type != 0)
      {
        socket_send(response.socket, &response);
      }
    }
  }
}