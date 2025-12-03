#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>
using namespace std;
class Connector
{
public:
    Connector();
    sql::ResultSet* GetResultPointer(const sql::SQLString& sql);
    void TestShow(sql::ResultSet* res);
private:
    sql::mysql::MySQL_Driver* driver;
    std::unique_ptr<sql::Connection> conn;
    std::string m_host;//数据库地址
    std::string m_user;//数据库用户名
    std::string m_password;//数据库密码

};

