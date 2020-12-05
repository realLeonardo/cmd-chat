/**
 * NOTE: 待扩展
 */
#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <iostream>
#include <string>

using std::string;

class message {
 public:
  message(int mid, string text, string create_at)
      : mid(mid), text(text), create_at(create_at) {}
  int mid;
  string text;
  string create_at;
};

#endif
