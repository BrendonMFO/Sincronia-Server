#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include <sys/sem.h>
#include <semaphore.h>

static sem_t sem_var;
static player_list_t players;
static long long players_count = 0;

player_list_node_t *player_list_find(long long player_id);

void player_list_init()
{
  sem_init(&sem_var, 0, 0);
  sem_post(&sem_var);
  players.first = NULL;
  players.last = NULL;
}

player_t *player_list_push(int player_socket, char *player_name)
{
  player_list_node_t *new_player_node = (player_list_node_t *)malloc(sizeof(player_list_node_t));
  if (new_player_node == NULL)
  {
    printf("Error: alloc player_node\n");
    exit(EXIT_FAILURE);
  }
  new_player_node->match_request = 0;
  new_player_node->player.socket = player_socket;
  strncpy(new_player_node->player.name, player_name, 20);
  new_player_node->next = NULL;

  sem_wait(&sem_var);
  new_player_node->player.id = players_count++;
  if (players.first == NULL && players.last == NULL)
  {
    new_player_node->prev = NULL;
    players.first = new_player_node;
    players.last = new_player_node;
  }
  else
  {
    players.last->next = new_player_node;
    new_player_node->prev = players.last;
    players.last = new_player_node;
  }
  sem_post(&sem_var);

  return &new_player_node->player;
}

void player_list_pop(player_list_node_t *player_node, int _free)
{
  sem_wait(&sem_var);

  if (players.first == player_node && players.last == player_node)
  {
    players.first = NULL;
    players.last = NULL;
  }
  else
  {
    if (player_node->prev != NULL)
    {
      player_node->prev->next = player_node->next;
    }
    else
    {
      players.first = player_node->next;
    }

    if (player_node->next != NULL)
    {
      player_node->next->prev = player_node->prev;
    }
    else
    {
      players.last = player_node->prev;
    }
  }

  if (_free)
  {
    free(player_node);
  }

  sem_post(&sem_var);
}

player_list_node_t *player_list_first(long long player_id)
{
  player_list_node_t *first;

  sem_wait(&sem_var);

  for (first = players.first; first != NULL; first = first->next)
  {
    if (first->player.id != player_id)
    {
      break;
    }
  }

  sem_post(&sem_var);

  if (first == NULL)
  {
    return NULL;
  }

  return first;
}

player_list_node_t *player_list_find(long long player_id)
{
  player_list_node_t *player_node;
  sem_wait(&sem_var);
  for (player_node = players.first; player_node != NULL; player_node = player_node->next)
  {
    if (player_node->player.id == player_id)
    {
      break;
    }
  }
  sem_post(&sem_var);
  return player_node;
}

void players_request_match_push(long long player_id)
{
  player_list_node_t *player_node = player_list_find(player_id);

  if (player_node == NULL)
  {
    return;
  }

  player_node->match_request = 1;
}