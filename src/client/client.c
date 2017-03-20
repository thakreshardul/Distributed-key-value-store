// protobuf-c headers
#include <protobuf-c/protobuf-c.h>
#include <protobuf-c-rpc/protobuf-c-rpc.h>
// Definitions for our protocol (under build/)
#include <protobuf/protocol.pb-c.h>

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

char *server_addr;

static void die (const char *format, ...)
{
  va_list args;
  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);
  fprintf (stderr, "\n");
  exit (1);
}

static void usage (void)
{
  die ("usage: example-client [--tcp=HOST:PORT]\n"
       "\n"
       "Run a protobuf client\n"
       "Options:\n"
       "  --tcp=HOST:PORT  Port to listen on for RPC clients.\n"
      );
}

static protobuf_c_boolean starts_with (const char *str, const char *prefix)
{
  return memcmp (str, prefix, strlen (prefix)) == 0;
}

static void handle_get_response (const Dkvs__Row *result, void *closure_data)
{
  if (result == NULL)
    printf ("Error processing request.\n");
  else if (result->key == NULL)
    printf ("Not found.\n");
  else
    printf ("%s:%s\n", result->key, result->value);
  * (protobuf_c_boolean *)closure_data = 1;
}

static void handle_set_response (const Dkvs__Status *result, void *closure_data)
{
  if (result == NULL)
    printf ("Error processing request.\n");
  else
    printf("%d\n", result->status);

  * (protobuf_c_boolean *)closure_data = 1;
}

static void handle_router_response (const Dkvs__RouterResponse *result, void *closure_data)
{
  if (result == NULL)
    printf ("Error processing request.\n");
  else if (result->address == NULL)
    printf ("No Server found.\n");
  else
  {
    printf ("%s\n", result->address);
    server_addr = (char *)malloc(sizeof(result->address));
    strcpy(server_addr, result->address);
  }
  * (protobuf_c_boolean *)closure_data = 1;
}

int main(int argc, char**argv)
{
  ProtobufCService *router_service, *server_service;
  ProtobufC_RPC_Client *client, *client_for_router;
  ProtobufC_RPC_AddressType address_type=0;
  const char *name = NULL;
  unsigned i;

  for (i = 1; i < (unsigned) argc; i++)
    {
      if (starts_with (argv[i], "--tcp="))
        {
          address_type = PROTOBUF_C_RPC_ADDRESS_TCP;
          name = strchr (argv[i], '=') + 1;
        }
      else
        usage ();
    }

  if (name == NULL)
    die ("missing --tcp=HOST:PORT");

  router_service = protobuf_c_rpc_client_new(address_type, name, &dkvs__router__descriptor, NULL);
  if (router_service == NULL)
    die ("error creating client");
  client_for_router = (ProtobufC_RPC_Client *) router_service;

router:
  fprintf (stderr, "Connecting to router\n");
  while (!protobuf_c_rpc_client_is_connected (client_for_router))
    protobuf_c_rpc_dispatch_run (protobuf_c_rpc_dispatch_default ());
  fprintf (stderr, "done.\n");

  char get[1024];
  Dkvs__RouterRequest query = DKVS__ROUTER_REQUEST__INIT;
  protobuf_c_boolean done = 0;
  printf("Enter the key\n>>");
  fgets (get, sizeof (get), stdin);
  char buf2[strlen(get)];
  strncpy(buf2, get, strlen(get)-1);
  query.key = buf2;
  printf("Got %sSending %s\n", get, query.key);
  dkvs__router__get_server (router_service, &query, handle_router_response, &done);
  while (!done)
    protobuf_c_rpc_dispatch_run (protobuf_c_rpc_dispatch_default ());  

  printf("%s\n", server_addr);
  server_service = protobuf_c_rpc_client_new(address_type, server_addr, &dkvs__server__descriptor, NULL);
  if (server_service == NULL)
    die ("error creating client");

  client = (ProtobufC_RPC_Client *) server_service;

  fprintf (stderr, "Connecting to server\n");
  while (!protobuf_c_rpc_client_is_connected (client))
    protobuf_c_rpc_dispatch_run (protobuf_c_rpc_dispatch_default());
  fprintf (stderr, "done.\n");

  for(;;)
  {  
    char command[1024];
    fprintf(stderr, "Please enter get or set\n");
    fprintf(stderr, ">>");
    if (fgets (command, sizeof (command), stdin) == NULL)
        break;
    else if (!(strcmp(command, "set\n")))
    {
      {
        protobuf_c_boolean is_done = 0;
        Dkvs__Row query = DKVS__ROW__INIT;
        char *key = (char *)malloc(1024);
        char *val = (char *)malloc(1024);
        fprintf(stderr, "Enter key\n>>");
        fgets(key, 1024, stdin);
        fprintf(stderr, "Enter value\n>>");
        fgets(val, 1024, stdin);
        char temp_key[strlen(key)];
        strncpy(temp_key, key, strlen(key)-1);
        char temp_value[strlen(val)];
        strncpy(temp_value, val, strlen(val)-1);
        query.key = temp_key;
        query.value = temp_value;
        dkvs__server__set(server_service, &query, handle_set_response, &is_done);
        while (!is_done)
          protobuf_c_rpc_dispatch_run (protobuf_c_rpc_dispatch_default ());
      }
    }
    else if (!(strcmp(command, "get\n")))
    {
      char buf[1024];
      Dkvs__RouterRequest query = DKVS__ROUTER_REQUEST__INIT;
      protobuf_c_boolean is_done = 0;
      printf("Enter the key\n>>");
      if (fgets (buf, 1024, stdin) == NULL)
        break;
      char buf2[strlen(buf)];
      strncpy(buf2, buf, strlen(buf)-1);
      query.key = buf2;
      printf("Got %sSending %s:%s\n", buf, query.key, buf2);
      dkvs__server__get (server_service, &query, handle_get_response, &is_done);
      while (!is_done)
        protobuf_c_rpc_dispatch_run (protobuf_c_rpc_dispatch_default ());
    }
    else
      goto router; 
  }
  
  return 0;
}
