#include <iostream>

#include "src/server.h"

vector<user> server::users({});

// NOTE: server launcher
int main(int, char**) {
  cout << "server started." << endl;
  server s;

  return 0;
}
