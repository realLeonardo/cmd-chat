#ifndef SERVER_H_
#define SERVER_H_

#include <arpa/inet.h>   // For inet_addr()
#include <fcntl.h>       // For fnctl()
#include <netdb.h>       // For gethostbyname()
#include <pthread.h>     // For pthread_create()
#include <stdlib.h>      // For exit()
#include <sys/socket.h>  // For socket(), connect(), send(), and recv()
#include <sys/types.h>   // For data types
#include <unistd.h>      // For close()

#include <cstring>  // For memset()
#include <iostream>
#include <string>
#include <vector>

#include "message.h"
#include "user.h"

using namespace std;

/* 监听的端口 */
#define SERV_PORT 9999
/* listen的请求接收队列长度 */
#define BACKLOG 128

class server {
 public:
  server();
  static vector<user> users;
};

#endif
