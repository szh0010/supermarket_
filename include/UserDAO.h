#pragma once
#include <string>
#include <memory>
#include "User.h"
#include "Connector.h"

// [yyx]
// 数据库层，这个用于和数据库交互
// DAO D database A acesss O Object
// 对数据库的数据进行增删改查
class UserDAO {
public:
    UserDAO(Connector& conn);

    bool GetUserByName(const std::string& username, User& outUser);
    bool ValidateLogin(const std::string& username, const std::string& password, User& outUser); // 验证用户是否登录成功

    bool AddUser(User& user);

private:
    Connector& connector;
};
