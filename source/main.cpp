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

int main() {
    // 设置 Windows 控制台为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    cout << "Starting MySQL demo..." << endl;

    try {
        // 1. 获取 MySQL 驱动
        //sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        // 2. 连接数据库 (用户名: root, 密码: 123456，可自行修改)
        unique_ptr<sql::Connection> conn(
            driver->connect("tcp://127.0.0.1:3306", "root", "123456")
        );

        cout << "Connected to MySQL!" << endl;

        // 3. 选择数据库 db
        conn->setSchema("db");

        // 4. 创建 Statement
        unique_ptr<sql::Statement> stmt(conn->createStatement());

        // 5. 执行 SQL 查询
        unique_ptr<sql::ResultSet> res(
            stmt->executeQuery(
                "SELECT username "
                "FROM testinfo"
            )
        );

        // 6. 遍历结果
        while (res->next()) {

            cout << "Username: " << res->getString("username") << endl;
            cout << "-----------------------------" << endl;
        }
    }
    catch (sql::SQLException& e) {
        cout << "# ERR: SQLException: " << e.what() << endl;
        cout << "MySQL error code: " << e.getErrorCode() << endl;
        cout << "SQLState: " << e.getSQLState() << endl;
    }

    return 0;
}

