#include "../include/Connector.h"
using namespace std;

int main() {
    // 设置 Windows 控制台为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Connector connector; //创建一个连接类
    string sql_t = "SELECT username FROM testinfo";
    sql::ResultSet* res_t = connector.GetResultPointer(sql_t);
    connector.TestShow(res_t);
    return 0;
}