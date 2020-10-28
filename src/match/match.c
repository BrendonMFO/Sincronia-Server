#include "match.h"
#include "player.h"
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

static sem_t sem_var;
static long long id = 0;
static match_list_t match_list;

void match_list_init()
{
  sem_init(&sem_var, 0, 0);
  sem_post(&sem_var);
  match_list.first = NULL;
  match_list.last = NULL;
}

match_t *match_push(player_list_node_t *player_one)
{
  player_list_node_t *player_two = player_list_first(player_one->player.id);

  player_one->match_request = 1;

  if (player_two == NULL || player_two->match_request == 0)
  {
    return NULL;
  }

  match_t *new_match = (match_t *)malloc(sizeof(match_t));

  if (new_match == NULL)
  {
    exit(EXIT_FAILURE);
  }

  new_match->id = id++;
  new_match->round = 0;
  new_match->player_one = (player_t *)malloc(sizeof(player_t));
  new_match->player_two = (player_t *)malloc(sizeof(player_t));
  memcpy(new_match->player_one, &player_one->player, sizeof(player_t));
  memcpy(new_match->player_two, &player_two->player, sizeof(player_t));

  player_list_pop(player_one, 1);
  player_list_pop(player_two, 1);

  sem_wait(&sem_var);
  if (match_list.first == NULL && match_list.last == NULL)
  {
    new_match->prev = NULL;
    new_match->next = NULL;
    match_list.first = new_match;
    match_list.last = new_match;
  }
  else
  {
    new_match->prev = match_list.last;
    match_list.last->next = new_match;
    match_list.last = new_match;
  }
  sem_post(&sem_var);

  return new_match;
}

void match_pop(long long match_id)
{
  match_t *match = match_find(match_id);

  if (match_list.first == match && match_list.last == NULL)
  {
    match_list.first = NULL;
    match_list.last = NULL;
  }
  else
  {
    if (match->prev != NULL)
    {
      match->prev->next = match->next;
    }
    else
    {
      match_list.first = match->next;
    }

    if (match->next != NULL)
    {
      match->next->prev = match->prev;
    }
    else
    {
      match_list.last = match->prev;
    }
  }

  free(match);
}

match_t *match_find(long long match_id)
{
  match_t *match;

  for (match = match_list.first; match != NULL; match = match->next)
  {
    if (match->id == match_id)
    {
      break;
    }
  }

  return match;
}