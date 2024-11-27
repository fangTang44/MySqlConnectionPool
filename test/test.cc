#include <iostream>
#include "MysqlConn.hpp"

int main() {
  MysqlConn* conn = new MysqlConn();
  conn->connect("root", "", "mysql", "127.0.0.1");
  conn->query("select * from plugin;");
  conn->next();
  std::cout << conn->value(0) << std::endl;
  std::cout << conn->value(1) << std::endl;
  std::cout << conn->value(2) << std::endl;
  std::cout << conn->value(3) << std::endl;
  // conn->next();
  // std::cout << conn->value(0) << std::endl;
  // std::cout << conn->value(1) << std::endl;
  delete conn;
}