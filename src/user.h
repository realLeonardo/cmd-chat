#ifndef USER_H_
#define USER_H_

#include <iostream>
#include <string>

using std::string;

class user {
 public:
  user(int uid, string name, string color)
      : uid(uid), name(name), color(color) {}
  int uid;
  string name;
  string color;
};

#endif
