#ifndef __PLAYER__
#define __PLAYER__

typedef struct s_player
{
  int socket;
  long long id;
  char name[20];
} player_t;

typedef struct s_player_list_node
{
  struct s_player_list_node *prev;
  player_t player;
  int match_request;
  struct s_player_list_node *next;
} player_list_node_t;

typedef struct s_player_list
{
  player_list_node_t *first;
  player_list_node_t *last;
} player_list_t;

void player_list_init();

player_t *player_list_push(int player_socket, char *player_name);

void player_list_pop(player_list_node_t *player_node, int _free);

player_list_node_t *player_list_first(long long player_id);

player_list_node_t *player_list_find(long long player_id);

#endif