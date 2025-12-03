#include "../include/Connector.h"
#include "../include/Menu.h"
using namespace std;


int main() {
    // 设置 Windows 控制台为 UTF-8
    //SetConsoleOutputCP(0);
    //SetConsoleCP(0);
    Connector connector; //创建一个连接类
    Menu menu;
    menu.ShowLoginMenu();
    // 我们需要取得用户名和密码，用于判断这个用户是否是管理员
    //menu.GetUsername();
    //menu.GetPassword();
    // 拿着用户名和密码去数据库比对出一个结果 难点
    
    // 假设我们这里判断比对出来了，这个用户是0
    int tempUserType = 0;
    // 然后把这个结果赋值给menu.

    menu.SetUserType(tempUserType);

    if (menu.GetUserType() == 0) // 管理员
    {
        // 进入管理员界面
        menu.ShowAdminMenu();
        int n;
        if (n == 1)
        {
            menu.ShowOpenMenu();
        }
        if (n == 2)
        {
            menu.ShowGiveMenu();
        }
        if (n == 3)
        {
            menu.ShowPassMenu();
        }
    }
    else // 顾客
    {
        // 进入顾客的界面
        menu.ShowGuestMenu();
        int n;
        if (n == 1)
        {
            menu.ShowShopMenu();
        }
        if (n == 2)
        {
            menu.ShowBackMenu();
        }
        if (n == 3)
        {
            menu.ShowMyselfMenu();
        }
    }
    // 判断是那种类型的用户，然后给出不同的界面
    //string sql_t = "SELECT username FROM testinfo";
    //sql::ResultSet* res_t = connector.GetResultPointer(sql_t);
    //connector.TestShow(res_t);

    
    return 0;
}