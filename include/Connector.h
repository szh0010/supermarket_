#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <memory>

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>
#include "Product.h"
using namespace std;
class Connector
{
public:
    // [yyx] 重构
    Connector();  // 自动读取 config.ini 并连接数据库
    ~Connector();  // 析构释放资源
    sql::ResultSet* Query(const std::string& sql); // 执行 SELECT 从数据库中查询数据
    int Execute(const std::string& sql);// 执行 INSERT/UPDATE/DELETE 

    std::unique_ptr<sql::PreparedStatement> Prepare(const std::string& sql);
    bool IsConnected() const { return conn != nullptr; }
    std::shared_ptr<sql::Connection> GetConnection();

    sql::ResultSet* GetResultPointer(const sql::SQLString& query);
    std::vector<Product> GetProducts();
    
private:
    // [yyx]
    void LoadConfig();               // 从 config.ini 读取配置
    bool Connect();                  // 连接数据库


private:
    // [yyx]
    std::string host;
    std::string user;
    std::string password;

    std::string database; 

    sql::mysql::MySQL_Driver* driver; 

    std::shared_ptr<sql::Connection> conn; // 这里用shared指针

};



