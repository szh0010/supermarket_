#pragma once
#include <string>

// [yyx]
// User 实体类
// 数据库读取用户数据，最后会存在这个类中
class User {
public:
    int id = 0;
    std::string username;
    std::string password;
    int user_type = 0;  // 0=顾客, 1=管理员
    int score = 0;
    int is_vip = 0;     // 0=非VIP, 1=VIP
    std::string gender; // "" 默认是空，所以这里注册要用户来选择
    int age = 0;
    std::string created_at;

    User() = default; // 默认构造函数
};
