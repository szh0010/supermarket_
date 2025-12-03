#include "../include/UserService.h"

UserService::UserService(UserDAO& dao)
    : dao(dao) {}

bool UserService::Login(const std::string& username, const std::string& password, User& userOut) {
    return dao.ValidateLogin(username, password, userOut);
}

bool UserService::Register(const User& user) {
    // 可在这里做检查，例如用户名是否重复
    User tmp;
    if (dao.GetUserByName(user.username, tmp)) {
        std::cout << "用户名已存在！" << std::endl;
        return false;
    }

    return dao.AddUser(user);
}
