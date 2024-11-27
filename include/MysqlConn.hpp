#ifndef MYSQLCONN_HPP
#define MYSQLCONN_HPP
#include <string>
#include "mysql/mysql.h"

class MysqlConn {
public:
  /// @brief 初始化数据库连接
  MysqlConn();

  /// @brief 释放数据库连接
  ~MysqlConn();

  /// @brief               连接数据库
  /// @param[in] sUser     MYSQL用户名
  /// @param[in] sPasswd   用户密码
  /// @param[in] sDbName   数据库名 
  /// @param[in] sIp       服务器IP地址 
  /// @param[in] usPort    服务端口号 
  /// @return              是否成功 
  bool connect(std::string sUser, std::string sPasswd, 
               std::string sDbName, std::string sIp, 
               unsigned short usPort = 3306);

  /// @brief           更新数据库
  /// @param[in] sSql  要执行的sql语句 
  /// @return          是否成功
  bool update(std::string sSql);

  /// @brief           查询数据库
  /// @param[in] sSql  要执行的sql语句
  /// @return          是否成功 
  bool query(std::string sSql);

  /// @brief          遍历查询得到结果集
  /// @return         是否得到结果
  bool next();

  /// @brief            得到结果集中的字段值
  /// @param[in] iIndex 获取第几个字段的值
  /// @return           具体值 
  std::string value(int iIndex);

  /// @brief            创建事务
  /// @return           是否成功创建 
  bool transaction();

  /// @brief            提交事务
  /// @return           是否成功提交 
  bool commit();

  /// @brief            事务回滚
  /// @return           是否成功回滚 
  bool rollback();
private:
  /// @brief            释放结果集中的数据以便下次使用
  void freeResult();

  MYSQL* m_pConn = nullptr;
  MYSQL_RES* m_pResult = nullptr;
  MYSQL_ROW m_pRow = nullptr;
};

#endif