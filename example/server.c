#include <reactor.h>
#include <err.h>

static void handle_request(reactor_event_t *event)
{
  if (event->type == SERVER_ERROR)
    err(1, "server callback error");
  server_plain((server_session_t *) event->data, string("hello"), NULL, 0);
}

int main()
{
  server_t server;

  reactor_construct();
  server_construct(&server, handle_request, NULL);
  server_open(&server, "127.0.0.1", 80);
  reactor_loop();
  reactor_destruct();
}
