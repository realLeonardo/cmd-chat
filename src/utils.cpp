#include "utils.h"

// NOTE: gen an name string
string utils::gen_username() {
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