#include "match.h"
#include "socket.h"
#include "player.h"
#include "events.h"
#include "communication.h"

int main(int argc, char const *argv[])
{
  socket_init();
  match_list_init();
  player_list_init();
  callback_list_init();
  events_register();
  socket_accept();
  return 0;
}