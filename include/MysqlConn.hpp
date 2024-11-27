#ifndef MYSQLCONN_HPP
#define MYSQLCONN_HPP
#include <string>
#include "mysql/mysql.h"

class MysqlConn {
public:
  // 初始化数据库连接
  MysqlConn();
  // 释放数据库连接
  ~MysqlConn();
  // 连接数据库
  bool connect(std::string sUser, std::string sPasswd, 
               std::string sDbName, std::string sIp, 
               unsigned short usPort = 3306);
  // 更新数据库 insert, update, delete
  bool update(std::string sSql);
  // 查询数据库
  bool query(std::string sSql);
  // 遍历查询得到结果集
  bool next();
  // 得到结果集中的字段值
  std::string value(int iIndex);
  // 事务操作
  bool transaction();
  // 事务提交
  bool commit();
  // 事务回滚
  bool rollback();
private:
  // 释放结果集中的数据
  void freeResult();

  MYSQL* m_pConn = nullptr;
  MYSQL_RES* m_pResult = nullptr;
  MYSQL_ROW m_pRow = nullptr;
};

#endif