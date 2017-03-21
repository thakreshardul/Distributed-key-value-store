Distributed key value store:

The task was to build a distributed key value store where, proxy kept the 
mapping of key and server and sent it to client when a key is requested.

The server and server1 have been initialised to different key value pairs.
Maximum capacity of the servers is 5.

The protocol:

The class has decided to use protobuf for unanimity.

Protobuf rpcs are used on router and server to contact with client.

Structure of protocol:

// Key/Value pair which is actually stored in the database
message Row {
  // `key_length` and `value_length` are deprecated
  // (Protobuf encodings of bytes are sized)
  optional uint64 key_length = 1;
  optional uint64 value_length = 2;
  required string key = 3;
  required string value = 4;
}

// Response from server on a 'Set' request (couldn't think of anything better)
enum StatusType {
  OK = 0;
  ERROR = 1;
}

message Status {
  required StatusType status = 1;
}

// Sent from clients to router to look up server associated with key
message RouterRequest {
  // `key_length` is deprecated (Protobuf encodings of bytes are sized)
  optional uint64 key_length = 1;
  required string key = 2;
}

message RouterResponse {
  // Address of server returned to client
  // (a string is used, since that is used in RPC connections)
  // The returned string should be a valid IP:PORT address.
  required string address = 1;
}

// RPCs implemented by router
service Router {
  /**
   * Returns a server which can handle get/set operations
   * associated with the given key.
   */
  rpc GetServer (RouterRequest) returns (RouterResponse);
}

// RPCs implemented by server nodes (for use with client)
service Server {
  /**
   * RPC which updates the hash table in this server with the
   * given key/value pair. Returns a status of OK if there were
   * no issues committing the given row.
   */
  rpc Set (Row) returns (Status);
  /**
   * RPC which returns the value in this server's table for the
   * given key. If the key is not found, NULL is returned.
   *
   * Aside: Maybe `RouterRequest` should be named something else?
   */
  rpc Get (RouterRequest) returns (Row);
}


Following is the flow of the application.

1. The client sends a key to the router.
2. If the key exists in any of the server, then router sends corresponding 
   server address to the client.
   If the key does not exist on any server, then router checks for a server 
   that is not completely fileed and returns its address to client.
3. The client then tries to connect to the server.
4. The client invokes get/set rpc to obtain key value pair or to add value
   to database.

Challenges faced:

1. Understanding of usage of protobuf in C.
2. Methods listed in the documentation of protobuf-c-rpc are outdated, which 
   costed a major chunk of time to figure out.
3. Implementing socket communication using protobuf.

Usage:

1. Run the makefile to build all the sources.
2. Run server and server on different ports(this ensures multiple servers)
   Usage is specified in the source code itself.
   Example:
   server --port=8887
   server1 --port=8888
3. Run Router process.
   router --port=7777
4. Run Client.
	client --tcp=127.0.0.1:7777
	Connecting to router...
	done.
	Enter the key to be searched
	>>test
	Server address:127.0.0.1:8887
	Connecting to server
	done.
	Please enter get or set
	>>get
	Enter the key
	>>test
	Key value pair: test:test
	Connecting to router...
	done.
	Enter the key to be searched
	>>hello
	Server address:127.0.0.1:8888
	Connecting to server
	done.
	Please enter get or set
	>>get
	Enter the key
	>>hello
	Key value pair: hello:world
	Connecting to router...
	done.
	Enter the key to be searched
	>>computer
	Server address:127.0.0.1:8887
	Connecting to server
	done.
	Please enter get or set
	>>set
	Enter key again
	>>computer
	Enter value
	>>systems
	Status for set rpc:1
	Connecting to router...
	done.
	Enter the key to be searched
	>>computer
	Server address:127.0.0.1:8887
	Connecting to server
	done.
	Please enter get or set
	>>get
	Enter the key
	>>computer
	Key value pair: computer:systems
	Connecting to router...
	done.
	Enter the key to be searched
