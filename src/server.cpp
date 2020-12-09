#include "server.h"

// NOTE: pthread_create 回调函数，格式如此
void *doServerRead(void *arg) {
  int cfd = *(int *)arg;

  while (true) {
    char buf[BUFSIZ];
    int ret = read(cfd, buf, sizeof buf);

    string name;
    for (auto c : server::users) {
      if (cfd == c.uid) {
        name = c.name;
      }
    }

    if (ret <= 0) {
      string leave_msg = name + " has left.";
      auto c_i = server::users.begin();

      for (auto beg = server::users.begin(); beg != server::users.end();
           ++beg) {
        // NOTE: 分发用户离开的消息
        if (cfd != (*beg).uid) {
          write((*beg).uid, leave_msg.c_str(), leave_msg.size());
        } else {
          c_i = beg;
        }
      }

      // NOTE: erase userinfo
      server::users.erase(c_i);
      cerr << name + " disconnect, left user count: " << server::users.size()
           << endl;

      break;
    }

    string msg = name + ": ";

    for (int i = 0; i < ret; ++i) msg += buf[i];

    // NOTE: server output for test
    cout << msg << endl;

    // NOTE: 分发用户消息
    for (auto c : server::users) {
      if (cfd != c.uid) {
        write(c.uid, msg.c_str(), msg.size());
      }
    }
  }
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

  if (listen(lfd, CLIENT_MAX) == -1) {
    cerr << "listen error" << endl;
    exit(1);
  }

  // NOTE: accept-loop
  while (true) {
    cfd = accept(lfd, (struct sockaddr *)&client_addr, &sin_size);

    if (cfd == -1) {
      cerr << "accept error" << endl;
      continue;
    }

    // NOTE: Generate a userinfo
    user new_user(cfd, utils::gen_username(), "");

    server::users.push_back(new_user);
    cout << "Got connection: " << new_user.name << endl;
    cout << "Current clients count: " << server::users.size() << endl;

    for (auto c : server::users) {
      // NOTE: 分发给其他 user
      if (cfd != c.uid) {
        string msg = new_user.name + " has joined.";
        write(c.uid, msg.c_str(), msg.size());
      } else {
        // NOTE: 返回自己的用户信息
        write(cfd, new_user.name.c_str(), new_user.name.size());
      }
    }

    // NOTE: readloop with multi thread
    pthread_t pt;
    pthread_create(&pt, NULL, &doServerRead, &cfd);
  }
}
