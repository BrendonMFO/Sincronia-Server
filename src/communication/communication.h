#ifndef __COMMUNICATION__
#define __COMMUNICATION__

#include "socket.h"

enum COMMUNICATION_TYPES
{
  PLAYER_CONNECT = 1,
  PLAYER_CONNECT_ACCEPT = 2,
  PLAYER_MATCH_REQUEST = 3,
  PLAYER_MATCH_RESPONSE = 4,
  PLAYER_MOVE = 5,
  PLAYER_OPPONENT_MOVE = 6,
  PLAYER_CAPTURE_HEXAGON = 7,
  PLAYER_OPPONENT_CAPTURE_HEXAGON = 8,
  PLAYER_TOGGLE_SINCRONIA = 9,
  PLAYER_OPPONENT_TOGGLE_SINCRONIA = 10,
  PLAYER_REQUEST_BATTLE = 11,
  PLAYER_OPPONENT_REQUEST_BATTLE = 12
};

typedef struct s_communication_callback
{
  struct s_communication_callback *prev;
  int type;
  void (*callback)(player_message_t *, player_message_t *);
  struct s_communication_callback *next;
} communication_callback_t;

typedef struct s_communication_callback_list
{
  communication_callback_t *first;
  communication_callback_t *last;
} communication_callback_list_t;

void callback_list_init();

void callback_list_push(int type, void (*callback)(player_message_t *, player_message_t *));

void callback_list_call(int type, player_message_t *message);

#endif