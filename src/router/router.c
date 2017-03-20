#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
// protobuf-c headers
#include <protobuf-c/protobuf-c.h>
#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <protobuf-c-rpc/protobuf-c-rpc-dispatch.h>
// Definitions for our protocol (under build/)
#include <protobuf/protocol.pb-c.h>

#define server_count 10

typedef struct _Router__Db Router__Db;
struct _Router__Db
{
	char addr[15];
	char *key;
};

static Router__Db database[server_count];
int database_entries = 0;
int db1_cap = 5;
int db2_cap = 5;

static void
die (const char *format, ...)
{
  va_list args;
  va_start (args, format);
  vfprintf (stderr, format, args);
  va_end (args);
  fprintf (stderr, "\n");
  exit (1);
}

static void
usage (void)
{
  die ("usage: server [--port=NUM | --unix=PATH]\n"
       "\n"
       "Run a protobuf server as specified by the get service\n"
       "in the protocol.proto file in the protobuf-c distribution.\n"
       "\n"
       "Options:\n"
       "  --port=NUM       Port to listen on for RPC clients.\n"
       "  --unix=PATH      Unix-domain socket to listen on.\n"
       "\n");
}

static protobuf_c_boolean starts_with (const char *str, const char *prefix)
{
  return memcmp (str, prefix, strlen (prefix)) == 0;
}

static int add_entry(char *key, char *value)
{
  if (database_entries < server_count)
  {
    database[database_entries].key = key;
    strcpy(database[database_entries].addr, value);
    if (!strcmp(database[database_entries].addr,"127.0.0.1:8887"))
    	db1_cap--;
    else
    	db2_cap--;
    database_entries++;

    return 1;
  }
  return 0;
}

static void initialize()
{
  add_entry("test", "127.0.0.1:8887");
  add_entry("test123", "127.0.0.1:8887");
  add_entry("shardul", "127.0.0.1:8887");
  add_entry("hello", "127.0.0.1:8888");
  add_entry("check", "127.0.0.1:8888");
  add_entry("kapil", "127.0.0.1:8888");
}



static void example__get_server (Dkvs__Router_Service *service,
                     const Dkvs__RouterRequest *input,
                     Dkvs__RouterResponse_Closure closure,
                     void *closure_data)
{
  (void) service;
  if (input == NULL || input->key == NULL)
    closure (NULL, closure_data);
  else
  {


    Dkvs__RouterResponse result = DKVS__ROUTER_RESPONSE__INIT;
    int found = 0;
    printf("In get server RPC\n");
    for (int i = 0; i < database_entries; i++)
    {
      printf("Searching for %s:%s\n", input->key, database[i].key);
      //printf("%d\n",strcmp(database[i].key, input->key));
      if(!(strcmp(database[i].key, input->key)))
      {
        printf("Match found\n");
        found++;
        result.address = database[i].addr;
        break;
      }
  	}
    if (found == 0)
    {  	
    	printf("Match not found...Maybe you want to set\n");
        if (db1_cap > 0)
        {
        	add_entry(input->key, "127.0.0.1:8887");
        	result.address = "127.0.0.1:8887";
        }
        else if (db2_cap > 0)
        {
        	add_entry(input->key, "127.0.0.1:8888");
        	result.address = "127.0.0.1:8888";
        }
        else
        	result.address = NULL;
    }
      
    printf("Sending %s\n", result.address);
    printf("Returning....\n");
    closure (&result, closure_data);
  }
}

static Dkvs__Router_Service router_service =
  DKVS__ROUTER__INIT(example__);

int main(int argc, char**argv)
{
  initialize();
  ProtobufC_RPC_Server *server;
  ProtobufC_RPC_AddressType address_type=0;
  const char *name = NULL;
  unsigned i;

  for (i = 1; i < (unsigned) argc; i++)
    {
      if (starts_with (argv[i], "--port="))
        {
          address_type = PROTOBUF_C_RPC_ADDRESS_TCP;
          name = strchr (argv[i], '=') + 1;
        }
      else
        usage ();
    }
  if (name == NULL)
    die("missing --port=NUM");

  server = protobuf_c_rpc_server_new(address_type, name, (ProtobufCService *) &router_service, NULL);

  for (;;)
    protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
  return 0;
}
