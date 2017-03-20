/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: protocol.proto */

#ifndef PROTOBUF_C_protocol_2eproto__INCLUDED
#define PROTOBUF_C_protocol_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Dkvs__Row Dkvs__Row;
typedef struct _Dkvs__Status Dkvs__Status;
typedef struct _Dkvs__RouterRequest Dkvs__RouterRequest;
typedef struct _Dkvs__RouterResponse Dkvs__RouterResponse;


/* --- enums --- */

/*
 * Response from server on a 'Set' request (couldn't think of anything better)
 */
typedef enum _Dkvs__StatusType {
  DKVS__STATUS_TYPE__OK = 0,
  DKVS__STATUS_TYPE__ERROR = 1
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(DKVS__STATUS_TYPE)
} Dkvs__StatusType;

/* --- messages --- */

/*
 * Key/Value pair which is actually stored in the database
 */
struct  _Dkvs__Row
{
  ProtobufCMessage base;
  /*
   * `key_length` and `value_length` are deprecated
   * (Protobuf encodings of bytes are sized)
   */
  protobuf_c_boolean has_key_length;
  uint64_t key_length;
  protobuf_c_boolean has_value_length;
  uint64_t value_length;
  char *key;
  char *value;
};
#define DKVS__ROW__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&dkvs__row__descriptor) \
    , 0,0, 0,0, NULL, NULL }


struct  _Dkvs__Status
{
  ProtobufCMessage base;
  Dkvs__StatusType status;
};
#define DKVS__STATUS__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&dkvs__status__descriptor) \
    , 0 }


/*
 * Sent from clients to router to look up server associated with key
 */
struct  _Dkvs__RouterRequest
{
  ProtobufCMessage base;
  /*
   * `key_length` is deprecated (Protobuf encodings of bytes are sized)
   */
  protobuf_c_boolean has_key_length;
  uint64_t key_length;
  char *key;
};
#define DKVS__ROUTER_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&dkvs__router_request__descriptor) \
    , 0,0, NULL }


struct  _Dkvs__RouterResponse
{
  ProtobufCMessage base;
  /*
   * Address of server returned to client
   * (a string is used, since that is used in RPC connections)
   * The returned string should be a valid IP:PORT address.
   */
  char *address;
};
#define DKVS__ROUTER_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&dkvs__router_response__descriptor) \
    , NULL }


/* Dkvs__Row methods */
void   dkvs__row__init
                     (Dkvs__Row         *message);
size_t dkvs__row__get_packed_size
                     (const Dkvs__Row   *message);
size_t dkvs__row__pack
                     (const Dkvs__Row   *message,
                      uint8_t             *out);
size_t dkvs__row__pack_to_buffer
                     (const Dkvs__Row   *message,
                      ProtobufCBuffer     *buffer);
Dkvs__Row *
       dkvs__row__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   dkvs__row__free_unpacked
                     (Dkvs__Row *message,
                      ProtobufCAllocator *allocator);
/* Dkvs__Status methods */
void   dkvs__status__init
                     (Dkvs__Status         *message);
size_t dkvs__status__get_packed_size
                     (const Dkvs__Status   *message);
size_t dkvs__status__pack
                     (const Dkvs__Status   *message,
                      uint8_t             *out);
size_t dkvs__status__pack_to_buffer
                     (const Dkvs__Status   *message,
                      ProtobufCBuffer     *buffer);
Dkvs__Status *
       dkvs__status__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   dkvs__status__free_unpacked
                     (Dkvs__Status *message,
                      ProtobufCAllocator *allocator);
/* Dkvs__RouterRequest methods */
void   dkvs__router_request__init
                     (Dkvs__RouterRequest         *message);
size_t dkvs__router_request__get_packed_size
                     (const Dkvs__RouterRequest   *message);
size_t dkvs__router_request__pack
                     (const Dkvs__RouterRequest   *message,
                      uint8_t             *out);
size_t dkvs__router_request__pack_to_buffer
                     (const Dkvs__RouterRequest   *message,
                      ProtobufCBuffer     *buffer);
Dkvs__RouterRequest *
       dkvs__router_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   dkvs__router_request__free_unpacked
                     (Dkvs__RouterRequest *message,
                      ProtobufCAllocator *allocator);
/* Dkvs__RouterResponse methods */
void   dkvs__router_response__init
                     (Dkvs__RouterResponse         *message);
size_t dkvs__router_response__get_packed_size
                     (const Dkvs__RouterResponse   *message);
size_t dkvs__router_response__pack
                     (const Dkvs__RouterResponse   *message,
                      uint8_t             *out);
size_t dkvs__router_response__pack_to_buffer
                     (const Dkvs__RouterResponse   *message,
                      ProtobufCBuffer     *buffer);
Dkvs__RouterResponse *
       dkvs__router_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   dkvs__router_response__free_unpacked
                     (Dkvs__RouterResponse *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Dkvs__Row_Closure)
                 (const Dkvs__Row *message,
                  void *closure_data);
typedef void (*Dkvs__Status_Closure)
                 (const Dkvs__Status *message,
                  void *closure_data);
typedef void (*Dkvs__RouterRequest_Closure)
                 (const Dkvs__RouterRequest *message,
                  void *closure_data);
typedef void (*Dkvs__RouterResponse_Closure)
                 (const Dkvs__RouterResponse *message,
                  void *closure_data);

/* --- services --- */

typedef struct _Dkvs__Router_Service Dkvs__Router_Service;
struct _Dkvs__Router_Service
{
  ProtobufCService base;
  void (*get_server)(Dkvs__Router_Service *service,
                     const Dkvs__RouterRequest *input,
                     Dkvs__RouterResponse_Closure closure,
                     void *closure_data);
};
typedef void (*Dkvs__Router_ServiceDestroy)(Dkvs__Router_Service *);
void dkvs__router__init (Dkvs__Router_Service *service,
                         Dkvs__Router_ServiceDestroy destroy);
#define DKVS__ROUTER__BASE_INIT \
    { &dkvs__router__descriptor, protobuf_c_service_invoke_internal, NULL }
#define DKVS__ROUTER__INIT(function_prefix__) \
    { DKVS__ROUTER__BASE_INIT,\
      function_prefix__ ## get_server  }
void dkvs__router__get_server(ProtobufCService *service,
                              const Dkvs__RouterRequest *input,
                              Dkvs__RouterResponse_Closure closure,
                              void *closure_data);
typedef struct _Dkvs__Server_Service Dkvs__Server_Service;
struct _Dkvs__Server_Service
{
  ProtobufCService base;
  void (*set)(Dkvs__Server_Service *service,
              const Dkvs__Row *input,
              Dkvs__Status_Closure closure,
              void *closure_data);
  void (*get)(Dkvs__Server_Service *service,
              const Dkvs__RouterRequest *input,
              Dkvs__Row_Closure closure,
              void *closure_data);
};
typedef void (*Dkvs__Server_ServiceDestroy)(Dkvs__Server_Service *);
void dkvs__server__init (Dkvs__Server_Service *service,
                         Dkvs__Server_ServiceDestroy destroy);
#define DKVS__SERVER__BASE_INIT \
    { &dkvs__server__descriptor, protobuf_c_service_invoke_internal, NULL }
#define DKVS__SERVER__INIT(function_prefix__) \
    { DKVS__SERVER__BASE_INIT,\
      function_prefix__ ## set,\
      function_prefix__ ## get  }
void dkvs__server__set(ProtobufCService *service,
                       const Dkvs__Row *input,
                       Dkvs__Status_Closure closure,
                       void *closure_data);
void dkvs__server__get(ProtobufCService *service,
                       const Dkvs__RouterRequest *input,
                       Dkvs__Row_Closure closure,
                       void *closure_data);

/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    dkvs__status_type__descriptor;
extern const ProtobufCMessageDescriptor dkvs__row__descriptor;
extern const ProtobufCMessageDescriptor dkvs__status__descriptor;
extern const ProtobufCMessageDescriptor dkvs__router_request__descriptor;
extern const ProtobufCMessageDescriptor dkvs__router_response__descriptor;
extern const ProtobufCServiceDescriptor dkvs__router__descriptor;
extern const ProtobufCServiceDescriptor dkvs__server__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_protocol_2eproto__INCLUDED */
