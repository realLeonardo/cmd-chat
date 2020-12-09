/**
 * NOTE: 模拟客户端
 */
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>  // For memset()
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/* 服务器端口 */
#define SERV_PORT 9999

// NOTE: messages vector
vector<string> msg_v;

// NOTE: pthread_create 回调函数，格式如此
void *doClientRead(void *arg) {
  int cfd = *(int *)arg;

  while (true) {
    char buf[BUFSIZ];
    int ret = read(cfd, buf, sizeof buf);

    if (ret <= 0) {
      cerr << "server read error" << endl;
      break;
    }

    string msg = "";
    for (int i = 0; i < ret; ++i) msg += buf[i];

    cout << msg << endl;
    msg_v.push_back(msg);
  }
}

void printMsgs() {
  for (auto msg : msg_v) {
    cout << msg << endl;
  }
}

int main(int argc, char *argv[]) {
  int cfd;
  struct sockaddr_in serv_addr;

  if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    cerr << "socket error" << endl;
    exit(1);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
  memset(&(serv_addr.sin_zero), 0, sizeof &(serv_addr.sin_zero));

  if (connect(cfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) ==
      -1) {
    cerr << "connect error" << endl;
    exit(1);
  }

  // NOTE: handle self info
  char buf[BUFSIZ];
  int ret = read(cfd, buf, sizeof buf);
  string name = "";
  for (int i = 0; i < ret; ++i) name += buf[i];

  system("clear");

  string first_msg = "Your name: " + name;
  msg_v.push_back(first_msg);
  cout << "Your name: " << name << endl;

  // NOTE: read loop with multi thread
  pthread_t pt;
  pthread_create(&pt, NULL, &doClientRead, &cfd);

  // NOTE: input loop
  while (true) {
    string text;
    getline(cin, text);

    if (text.size() != 0) {
      write(cfd, text.c_str(), text.size());
      msg_v.push_back(name + ": " + text);
    }

    system("clear");
    printMsgs();
  }
}