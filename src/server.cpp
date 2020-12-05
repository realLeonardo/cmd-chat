#include "server.h"

// NOTE: pthread_create 回调函数，格式如此
void *doServerRead(void *arg) {
  int cfd = *(int *)arg;

  while (true) {
    char buf[BUFSIZ];
    int ret = read(cfd, buf, sizeof buf);

    for (int i = 0; i < ret; ++i) cout << buf[i];
    cout << endl;

    string msg;
    int name_len = 0;
    for (auto c : server::users) {
      if (cfd == c.uid) {
        msg = c.name + ": ";
        name_len = msg.size();
      }
    }

    for (int i = 0; i < ret; ++i) msg += buf[i];

    for (auto c : server::users) {
      if (cfd != c.uid) {
        write(c.uid, msg.c_str(), ret + name_len);
      }
    }
  }
}

// NOTE: gen an name string
string gen_username() {
  static int i = 0;

  vector<string> names;
  names.push_back("Alice");
  names.push_back("Bob");
  names.push_back("Clera");
  names.push_back("Dente");
  names.push_back("Einstan");
  names.push_back("Frank");
  names.push_back("Gergeo");
  // and so on...

  return names[i++];
}

server::server() {
  int lfd, cfd;
  // Server address struct
  struct sockaddr_in serv_addr;
  struct sockaddr_in client_addr;
  unsigned int sin_size = sizeof(struct sockaddr_in);

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    cerr << "socket error" << endl;
    exit(1);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  memset(&(serv_addr.sin_zero), 0, sizeof &(serv_addr.sin_zero));

  if (bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    cerr << "bind error" << endl;
    exit(1);
  }

  if (listen(lfd, BACKLOG) == -1) {
    cerr << "listen error" << endl;
    exit(1);
  }

  // NOTE: loop accepts
  while (true) {
    cfd = accept(lfd, (struct sockaddr *)&client_addr, &sin_size);

    if (cfd == -1) {
      cerr << "accept error" << endl;
      continue;
    }

    user u(cfd, gen_username(), "");
    server::users.push_back(u);
    cout << "Got connection: " << u.name << endl;
    cout << "Current clients count: " << server::users.size() << endl;

    for (auto c : server::users) {
      // NOTE: 分发给其他 user
      if (cfd != c.uid) {
        string msg = u.name + ": Hello everyone~";
        write(c.uid, msg.c_str(), msg.size());
      } else {
        // NOTE: 返回自己的用户信息
        write(cfd, u.name.c_str(), u.name.size());
      }
    }

    // NOTE: readloop base on multi thread
    pthread_t pt;
    pthread_create(&pt, NULL, &doServerRead, &cfd);
  }
}
