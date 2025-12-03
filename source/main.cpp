#include "../include/Config.h" // [yyx] 读取文件夹下的config.ini 配置文件
#include "../include/Connector.h"
#include "../include/Menu.h"
#include "../include/User.h"
#include "../include/UserDAO.h"
#include "../include/UserService.h"
using namespace std;


int main() {
    
    Config::Instance().Load("../config.ini");
    // 初始化数据库连接
    Connector db;
    UserDAO userDao(db); // 将用户的数据库层打开
    UserService userService(userDao); // 将用户的业务层打开
    if (!db.IsConnected()) {
        return -1;
    }
    Menu menu;
    menu.ShowLoginMenu();
    string username = menu.GetUsername();
    string password = menu.GetPassword();
    User loginUser; // 用户登录

    // 下面这块逻辑还需要处理
    if (userService.Login(username, password, loginUser)) {
        cout << "登录成功!" << endl;
        cout << "欢迎: " << loginUser.username << endl;

        if (loginUser.user_type == 1)
            menu.ShowAdminMenu();
        else
            menu.ShowGuestMenu();
    }
    else {
        cout << "账号或密码错误！" << endl;
    }
    
    return 0;
}