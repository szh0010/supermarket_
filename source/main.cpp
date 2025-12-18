#include "../include/Config.h" // [yyx] 读取文件夹下的config.ini 配置文件
#include "../include/Connector.h"
#include "../include/Menu.h"
#include "../include/User.h"
#include "../include/UserDAO.h"
#include "../include/UserService.h"
#include "../include/ProductDAO.h"
#include "../include/ProductService.h"

using namespace std;

void HandleLogin(Menu& menu, UserService& userService, User& loginUser,ProductService& productService)
{
    menu.ShowLoginMenu();
    string username = menu.GetUsername();
    string password = menu.GetPassword();
    if (userService.Login(username, password, loginUser))
    {
        cout << "登录成功!" << endl;
        cout << "欢迎" << loginUser.username << endl;
        if (loginUser.user_type == 1)
        {
            menu.ShowAdminMenu();
        }
        else
        {
            menu.ShowGuestMenu(productService);
        }
    }
    else
    {
        cout << "账号或密码错误！" << endl;
    }
}

void HandleRegister(Menu& menu, UserService& userService, User& registerUser, ProductService& productService)
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
        HandleLogin(menu, userService, registerUser,productService);
    }
    else
   {
        cout << "注册失败，请重试！" << endl;
    }
}

int main() {


    Config::Instance().Load("E:/c++code/supermarket/config.ini");

    // 初始化数据库连接
    Connector db;
    UserDAO userDao(db); // 将用户的数据库层打开
    UserService userService(userDao); // 将用户的业务层打开
    ProductDAO productDao(db);
    ProductService productService(productDao);
    
    if (!db.IsConnected()) {
        return -1;
    }
    Menu menu;

    menu.ShowEnterMenu();
    string username = menu.GetUsername();
    string password = menu.GetPassword();
    User loginUser; // 用户登录
    User registerUser; //用户注册
    int choice;
    cin >> choice;
    if (choice == 1)
    {
        HandleLogin(menu, userService, loginUser, productService);
    }
    else if (choice == 2)
    {
        HandleRegister(menu, userService, registerUser, productService);
    }
    else
    {
        cout << "输入有误" << endl;
    }


    return 0;
}