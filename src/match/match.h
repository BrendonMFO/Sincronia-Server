#ifndef __MATCH__
#define __MATCH__

#include "player.h"

typedef struct s_match
{
  struct s_match *prev;
  int round;
  long long id;
  player_t *player_one;
  player_t *player_two;
  struct s_match *next;
} match_t;

typedef struct s_match_list
{
  match_t *first;
  match_t *last;
} match_list_t;

void match_list_init();

match_t *match_push(player_list_node_t *player_one);

void match_pop(long long match_id);

match_t *match_find(long long match_id);

#endif