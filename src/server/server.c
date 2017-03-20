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

#define database_size 10
static Dkvs__Row database[database_size];
int database_entries = 0;


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

/*static int compare_key (const void *a, const void *b)
{
  printf("Comparing keys\n");
  int ans = strcmp (a, ((const Dkvs__Row *)b)->key);
  return ans; 
}
*/

static int add_entry(char *key, char *value)
{
  if (database_entries < database_size)
  {
    database[database_entries].key = key;
    database[database_entries].value = value;
    database_entries++;
    return 1;
  }
  return 0;
}

static void initialize()
{
  add_entry("test", "test");
  add_entry("test123", "test123");
  add_entry("shardul", "thakre");
}

static void example__get (Dkvs__Server_Service *service,
              const Dkvs__RouterRequest *input,
              Dkvs__Row_Closure closure,
              void *closure_data)
{
  (void) service;
  if (input == NULL || input->key == NULL)
    closure (NULL, closure_data);
  else
  {

    Dkvs__Row result = DKVS__ROW__INIT;

    
    printf("In get RPC\n");
    for (int i = 0; i < database_entries; i++)
    {
      printf("Searching for %s:%s\n", input->key, database[i].key);
      printf("%d\n",strcmp(database[i].key, input->key));
      if(!(strcmp(database[i].key, input->key)))
      {
        printf("Match found\n");
        result.key = database[i].key;
        result.value = database[i].value;
        break;
      }
    }

    printf("Sending %s:%s\n", result.key, result.value);
    printf("Returning....\n");
    closure (&result, closure_data);
  }
}

static void example__set (Dkvs__Server_Service *service,
                          const Dkvs__Row *input,
                          Dkvs__Status_Closure closure,
                          void *closure_data)
{
  (void) service;
  Dkvs__Status result = DKVS__STATUS__INIT;
  if (input == NULL || input->key == NULL)
  {
    result.status = 0;
    closure (NULL, closure_data);
  }
  else
  {
    printf("In set RPC\n");
    char *key = (char *)malloc(1024);
    char *value = (char *)malloc(1024);
    key = memcpy(key, input->key, sizeof(input->key));
    value = memcpy(value, input->value, sizeof(input->value));
    int found = 0, written = 0;
    for (int i = 0; i < database_entries; i++)
    {
      printf("Searching for %s\n", key);
      if(!(strcmp(database[i].key,key)))
      {
        printf("Match found\n");
        database[i].key = key;
        database[i].value = value; 
        printf("Written successfully\n");
        found++;
      }
    }
    if (!found && add_entry(key, value))
    {
      printf("No Match found so new written\n");
      result.status = 1;
      written++;
    }
    if (written == 0 && found == 0)
    {
      printf("Some error in writing\n");  
      result.status = 0;
    }

    closure (&result, closure_data);
}
}

static Dkvs__Server_Service server_services =
  DKVS__SERVER__INIT(example__);

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

  server = protobuf_c_rpc_server_new(address_type, name, (ProtobufCService *) &server_services, NULL);

  for (;;)
    protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
  return 0;
}
