#include "MysqlConn.hpp"

MysqlConn::MysqlConn() {
  m_pConn = mysql_init(nullptr);
  mysql_set_character_set(m_pConn, "utf8");
}

MysqlConn::~MysqlConn() {
  if (m_pConn) {
    mysql_close(m_pConn);
    m_pConn = nullptr;
  }
  freeResult();
}

bool MysqlConn::connect(std::string sUser, std::string sPasswd, 
                        std::string sDbName, std::string sIp, 
                        unsigned short usPort) {
  auto* pPtr = mysql_real_connect(m_pConn, sIp.c_str(), 
                     sUser.c_str(), sPasswd.c_str(), 
                     sDbName.c_str(), usPort, nullptr, 0);
  return pPtr ? true: false;
}

bool MysqlConn::update(std::string sSql) {
  if (mysql_query(m_pConn, sSql.c_str())) {
    return false;
  }
  return true;
}

bool MysqlConn::query(std::string sSql) {
  freeResult();
  if (mysql_query(m_pConn, sSql.c_str())) {
    return false;
  }
  m_pResult = mysql_store_result(m_pConn);
  return true;
}

bool MysqlConn::next() {
  if (m_pResult != nullptr) {
    m_pRow = mysql_fetch_row(m_pResult);
    return true;
  }
  return false;
}

std::string MysqlConn::value(int iIndex) {
  int rowCount = mysql_num_fields(m_pResult);
  if (iIndex > rowCount || iIndex < 0) {
    return std::string();
  }
  char* val = m_pRow[iIndex];
  // 防止因为数据中存在'\0'导致数据读取不完全
  unsigned long length = mysql_fetch_lengths(m_pResult)[iIndex];
  return std::string(val, length);
}

bool MysqlConn::transaction() {
  return mysql_autocommit(m_pConn, false);
}

bool MysqlConn::commit() {
  return mysql_commit(m_pConn);
}

bool MysqlConn::rollback() {
  return mysql_rollback(m_pConn);
}

void MysqlConn::freeResult() {
  if (m_pResult) {
    mysql_free_result(m_pResult);
    m_pResult = nullptr;
  }
}
