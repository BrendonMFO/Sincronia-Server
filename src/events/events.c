#include "match.h"
#include "events.h"
#include "socket.h"
#include "player.h"
#include <string.h>
#include "communication.h"

void emit_match_begin(match_t *match);
void on_player_connection(player_message_t *request, player_message_t *response);
void on_player_move_request(player_message_t *request, player_message_t *response);
void on_player_match_request(player_message_t *message, player_message_t *response);
void on_player_request_battle(player_message_t *request, player_message_t *response);
void on_player_capture_hexagon(player_message_t *request, player_message_t *response);
void on_player_toggle_sincronia(player_message_t *request, player_message_t *response);

void events_register()
{
  callback_list_push(PLAYER_MOVE, on_player_move_request);
  callback_list_push(PLAYER_CONNECT, on_player_connection);
  callback_list_push(PLAYER_MATCH_REQUEST, on_player_match_request);
  callback_list_push(PLAYER_REQUEST_BATTLE, on_player_request_battle);
  callback_list_push(PLAYER_CAPTURE_HEXAGON, on_player_capture_hexagon);
  callback_list_push(PLAYER_TOGGLE_SINCRONIA, on_player_toggle_sincronia);
}

void on_player_connection(player_message_t *request, player_message_t *response)
{
  player_t *player = player_list_push(request->socket, request->content.player_connection.name);
  response->socket = request->socket;
  response->type = PLAYER_CONNECT_ACCEPT;
  response->content.player_connection_accept.id = player->id;
}

void on_player_match_request(player_message_t *request, player_message_t *response)
{
  match_t *match = match_push(player_list_find(request->content.player_request_match.id));
  response->socket = request->socket;
  response->type = PLAYER_MATCH_RESPONSE;
  if (match == NULL)
  {
    response->content.player_request_match_response.created = 0;
  }
  else
  {
    response->content.player_request_match_response.created = 1;
    response->content.player_request_match_response.player_pos = 1;
    response->content.player_request_match_response.match_id = match->id;
    response->content.player_request_match_response.opponent_socket = match->player_two->socket;
    emit_match_begin(match);
  }
}

void on_player_move_request(player_message_t *request, player_message_t *response)
{
  player_message_t send_player;
  send_player.type = PLAYER_OPPONENT_MOVE;
  send_player.content.player_request_move.hexagon = request->content.player_request_move.hexagon;
  socket_send(request->content.player_request_move.opponent_socket, &send_player);
}

void on_player_capture_hexagon(player_message_t *request, player_message_t *response)
{
  player_message_t send_player;
  send_player.type = PLAYER_OPPONENT_CAPTURE_HEXAGON;
  send_player.content.player_capture_hexagon.element = request->content.player_capture_hexagon.element;
  send_player.content.player_capture_hexagon.hexagon = request->content.player_capture_hexagon.hexagon;
  socket_send(request->content.player_capture_hexagon.opponent_socket, &send_player);
}

void on_player_toggle_sincronia(player_message_t *request, player_message_t *response)
{
  player_message_t send_player;
  send_player.type = PLAYER_OPPONENT_TOGGLE_SINCRONIA;
  send_player.content.player_toggle_sincronia.active = request->content.player_toggle_sincronia.active;
  socket_send(request->content.player_toggle_sincronia.opponent_socket, &send_player);
}

void on_player_request_battle(player_message_t *request, player_message_t *response)
{
  player_message_t send_player;
  send_player.type = PLAYER_OPPONENT_REQUEST_BATTLE;
  send_player.content.player_request_battle.target_hexagon = request->content.player_request_battle.target_hexagon;
  socket_send(request->content.player_request_battle.opponent_socket, &send_player);
}

void emit_match_begin(match_t *match)
{
  player_message_t send_player;
  send_player.type = PLAYER_MATCH_RESPONSE;
  send_player.content.player_request_match_response.created = 1;
  send_player.content.player_request_match_response.player_pos = 2;
  send_player.content.player_request_match_response.match_id = match->id;
  send_player.content.player_request_match_response.opponent_socket = match->player_one->socket;
  socket_send(match->player_two->socket, &send_player);
}