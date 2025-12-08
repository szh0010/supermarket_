#include "../include/UserDAO.h"
#include <jdbc/cppconn/prepared_statement.h>

UserDAO::UserDAO(Connector& conn)
    : connector(conn) {}

bool UserDAO::GetUserByName(const std::string& username, User& outUser) {
    try {
        auto connPtr = connector.GetConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            connPtr->prepareStatement("SELECT * FROM UserInfo WHERE username = ?")
        );
        stmt->setString(1, username); // 这里的 1 和第一个 ? 也就是"username = ?"是对应的关系

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        if (!res->next()) {
            return false;
        }
        // 解析取得的结果，并且把结果赋值给outUser
        // 思考题，为什么这里可以把结果赋值给outUser？
        // 下次提交的时候回答写在这里：
        outUser.id = res->getInt("id");
        outUser.username = res->getString("username");
        outUser.password = res->getString("password");
        outUser.user_type = res->getInt("user_type");
        outUser.score = res->getInt("score");
        outUser.is_vip = res->getInt("is_vip");
        outUser.gender = res->getString("gender");
        outUser.age = res->getInt("age");
        outUser.created_at = res->getString("created_at");

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "GetUserByName SQLException: " << e.what() << std::endl;
        return false;
    }
}

bool UserDAO::ValidateLogin(const std::string& username, const std::string& password, User& outUser) {
    try {
        auto connPtr = connector.GetConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            connPtr->prepareStatement(
                "SELECT * FROM UserInfo WHERE username = ? AND password = ?"
            )
        );
        // 和GetUserByName是同样的道理
        stmt->setString(1, username); // 这里的 1 和第一个 ? 也就是"username = ?"是对应的关系
        stmt->setString(2, password); // 这里的 2 和第二个 ? 也就是"password = ?"是对应的关系

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        if (!res->next()) {
            return false;
        }
        // 思考题：
        // 1. 这里是如何判断用户成功登录的？
        // 2. 如果登录失败了，说明是什么情况？
        // 3. 登录失败还会把outUser给赋值吗？
        // 作答区：
<<<<<<< HEAD
        // 1. 
        // 2.
        // 3.
=======
        // 1. 通过用户输入的账号密码与数据库进行比对，数据一致则登录成功
        // 2. 登录失败说明账号或者密码有误
        // 3. 不会
>>>>>>> a7b4713 (鍒濇瀹屾垚瓒呭競娉ㄥ唽绯荤粺鐨勬鏋?
        outUser.id = res->getInt("id");
        outUser.username = res->getString("username");
        outUser.password = res->getString("password");
        outUser.user_type = res->getInt("user_type");
        outUser.score = res->getInt("score");
        outUser.is_vip = res->getInt("is_vip");
        outUser.gender = res->getString("gender");
        outUser.age = res->getInt("age");
        outUser.created_at = res->getString("created_at");

        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "ValidateLogin SQLException: " << e.what() << std::endl;
        return false;
    }
}

// 此函数为后续注册用户使用，将用户的信息写入数据库中
bool UserDAO::AddUser(User& user) {
    try {
        auto connPtr = connector.GetConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(
            connPtr->prepareStatement(
                "INSERT INTO UserInfo (username, password, user_type, score, is_vip, gender, age, created_at) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, NOW())"
            )
        );

        stmt->setString(1, user.username);
        stmt->setString(2, user.password);
        stmt->setInt(3, user.user_type);
        stmt->setInt(4, user.score);
        stmt->setInt(5, user.is_vip);
        stmt->setString(6, user.gender);
        stmt->setInt(7, user.age);

        stmt->executeUpdate();
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "AddUser SQLException: " << e.what() << std::endl;
        return false;
    }
}
