#pragma once
#include "UserDAO.h"

// 业务逻辑层
// Service = 服务
class UserService {
public:
    UserService(UserDAO& dao);

    bool Login(const std::string& username, const std::string& password, User& userOut); // 登录用户逻辑

    //bool Register(const User& user); // 注册用户逻辑
    bool Register(User& user);


private:
    UserDAO& dao;
};
