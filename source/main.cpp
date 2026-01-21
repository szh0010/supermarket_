#include "../include/Config.h" 
#include "../include/Connector.h"
#include "../include/Menu.h"
#include "../include/User.h"
#include "../include/UserDAO.h"
#include "../include/UserService.h"
#include "../include/ProductDAO.h"
#include <windows.h>
#include <iostream>

using namespace std;

// 修改：增加 Connector& db 参数，以便传递给 menu 函数
void HandleLogin(Menu& menu, UserService& userService, User& loginUser, ProductService& productService, Connector& db)
{
    menu.ShowLoginMenu();
    string username = menu.GetUsername();
    string password = menu.GetPassword();

    if (userService.Login(username, password, loginUser))
    {
        cout << "登录成功!" << endl;
        cout << "欢迎 " << loginUser.username << endl;

        // 假设 user_type 1 为管理员，其他为顾客
        if (loginUser.user_type == 1)
        {
            menu.ShowAdminMenu();
        }
        else
        {
            // 修复 C2660：传入 db 参数，支持购买记录存储功能
            menu.ShowGuestMenu(productService, db);
        }
    }
    else
    {
        cout << "账号或密码错误！" << endl;
    }
}

// 修改：增加 Connector& db 参数
void HandleRegister(Menu& menu, UserService& userService, User& registerUser, ProductService& productService, Connector& db)
{
    menu.ShowRegisterMenu();
    string username = menu.GetUsername();
    string password = menu.GetPassword();
    string gender = menu.GetGender();

    registerUser.username = username;
    registerUser.password = password;
    registerUser.gender = gender;

    if (userService.Register(registerUser))
    {
        cout << "注册成功！" << endl;
        // 注册成功后自动登录，同样需要传入 db
        HandleLogin(menu, userService, registerUser, productService, db);
    }
    else
    {
        cout << "注册失败，请重试！" << endl;
    }
}

int main() {
    // 设置控制台为 UTF-8 编码，防止中文乱码
    SetConsoleOutputCP(65001);

    // 读取配置文件
    Config::Instance().Load("E:/c++code/supermarket/config.ini");

    // 初始化数据库连接
    Connector db;
    if (!db.IsConnected()) {
        cout << "数据库连接失败，请检查配置！" << endl;
        return -1;
    }

    // 初始化业务层和 DAO 层
    UserDAO userDao(db);
    UserService userService(userDao);
    ProductDAO productDao(db);
    ProductService productService(productDao);

    Menu menu;

    menu.ShowEnterMenu();

    int choice;
    if (!(cin >> choice)) return 0;

    User loginUser;    // 用于存放登录成功的用户信息
    User registerUser; // 用于存放新注册的用户信息

    if (choice == 1)
    {
        // 传入 db 以支持后续的购买记录操作
        HandleLogin(menu, userService, loginUser, productService, db);
    }
    else if (choice == 2)
    {
        // 传入 db
        HandleRegister(menu, userService, registerUser, productService, db);
    }
    else
    {
        cout << "输入有误" << endl;
    }

    system("pause");
    return 0;
}