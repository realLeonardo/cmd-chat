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
#include "utils.h"

using namespace std;

/* 监听端口 */
#define SERV_PORT 9999
/* 最大客户端连接数 */
#define CLIENT_MAX 128

class server {
 public:
  server();
  void *test(void *arg);

  // NOTE: 为了 doServerRead 能够获取到数据，暂时设为静态。
  // 是否有更好的方法？
  static vector<user> users;
};

#endif
