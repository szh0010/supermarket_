#include "../include/Menu.h"
#include "../include/Product.h"
#include "../include/Connector.h"
#include <vector>
#include <iomanip>
#include <iostream>

using namespace std;

// --- 基础 getter/setter 实现 (补齐链接缺失的符号) ---

// 必须实现这些函数，否则 main.cpp 调用时会报 LNK2001
string Menu::GetUsername() { return this->username; }
string Menu::GetPassword() { return this->password; }
string Menu::GetGender() { return this->gender; }
int Menu::GetUserType() { return this->userType; }

void Menu::SetGender(const string gender) { this->gender = gender; }
void Menu::SetUserType(int userType) { this->userType = userType; }

// --- 基础登录与注册功能 ---

void Menu::ShowEnterMenu() {
    cout << "----- 欢迎使用晶东超市系统 ------" << endl;
    cout << "1.老用户登录" << endl << "2.新用户注册" << endl;
}

void Menu::ShowLoginMenu() {
    cout << "----- 欢迎使用晶东超市系统 ------" << endl;
    cout << "请输入用户名: ";
    cin >> this->username;
    cout << "请输入密码: ";
    cin >> this->password;
    system("cls");
}

void Menu::ShowRegisterMenu() {
    cout << "----- 欢迎使用注册系统 ------" << endl;
    cout << "请输入用户名: ";
    cin >> this->username;
    cout << "请输入密码: ";
    cin >> this->password;
    cout << "请选择你的性别 (1.男 2.女): ";
    int choice; cin >> choice;
    SetGender(choice == 1 ? "male" : "female");
    system("cls");
}

// 补齐 ShowAdminMenu 实现
void Menu::ShowAdminMenu() {
    cout << "----- 欢迎使用晶东超市管理系统 ------" << endl;
    cout << "----- 1.查看超市经营情况 ------------" << endl;
    cout << "----- 2.货物补充 --------------------" << endl;
    cout << "----- 3.过期货物处理 ----------------" << endl;
    cout << "----- 0.退出登录 --------------------" << endl;
}

// --- 顾客主逻辑流程 ---

void Menu::ShowGuestMenu(ProductService productService, Connector& db) {
    while (true) {
        system("cls");
        cout << "----- 欢迎光临晶东超市 (" << this->username << ") ------" << endl;
        cout << "1. 选购商品\n2. 退换商品\n3. 我的账户\n0. 退出登录" << endl;
        int num; cin >> num;
        if (num == 1) {
            vector<Product> products;
            productService.GetAllProducts(products);
            ShowShopMenu(products, db);
        }
        else if (num == 2) ShowBackMenu(db);
        else if (num == 3) ShowMyselfMenu(db);
        else if (num == 0) break;
    }
}

// 1. 选购商品
void Menu::ShowShopMenu(const vector<Product>& products, Connector& db) {
    system("cls");
    cout << "----- 商品列表 -----" << endl;
    if (products.empty()) { cout << "暂无商品。" << endl; system("pause"); return; }
    for (const auto& product : products) product.Print();

    cout << "\n请输入商品编号购买 (0返回): ";
    int productId; cin >> productId;
    if (productId == 0) return;

    for (const auto& product : products) {
        if (product.GetId() == productId) {
            if (product.GetStock() > 0) {
                try {
                    db.Execute("UPDATE products SET stock = stock - 1 WHERE id = " + to_string(productId));
                    auto pstmt = db.Prepare("INSERT INTO buy_records(username, product_id, product_name, buy_price) VALUES (?, ?, ?, ?)");
                    pstmt->setString(1, this->username);
                    pstmt->setInt(2, product.GetId());
                    pstmt->setString(3, product.GetName());
                    pstmt->setDouble(4, product.GetPrice());
                    pstmt->executeUpdate();

                    int earned = static_cast<int>(product.GetPrice());
                    db.Execute("UPDATE userinfo SET score = score + " + to_string(earned) + " WHERE username = '" + this->username + "'");
                    cout << "【购买成功】获得积分: " << earned << " 分！" << endl;
                }
                catch (sql::SQLException& e) { cerr << "数据库异常: " << e.what() << endl; }
            }
            else cout << "【购买失败】库存不足！" << endl;
            break;
        }
    }
    system("pause");
}

// 2. 退换商品
void Menu::ShowBackMenu(Connector& db) {
    system("cls");
    cout << "----- 退换商品中心 (" << this->username << ") -----" << endl;

    struct Entry {
        int rid;
        int pid;
        string name;
        double price;
    };
    vector<Entry> history;

    try {
        sql::ResultSet* res = db.Query("SELECT record_id, product_id, product_name, buy_price, buy_time FROM buy_records WHERE username = '" + this->username + "' ORDER BY buy_time DESC");
        if (!res || res->rowsCount() == 0) {
            cout << "暂无购买记录。" << endl;
            if (res) delete res;
            system("pause"); return;
        }

        cout << left << setw(6) << "序号" << setw(20) << "名称" << "金额" << endl;
        int idx = 1;
        while (res->next()) {
            cout << left << setw(6) << idx << setw(20) << res->getString("product_name") << res->getDouble("buy_price") << endl;
            double currentPrice = static_cast<double>(res->getDouble("buy_price"));

            history.push_back({
                res->getInt("record_id"),
                res->getInt("product_id"),
                res->getString("product_name"),
                currentPrice
                });
            idx++;
        }
        delete res;

        cout << "\n请输入退换序号 (0返回): ";
        int choice; cin >> choice;
        if (choice > 0 && choice < idx) {
            Entry s = history[choice - 1];
            db.Execute("UPDATE products SET stock = stock + 1 WHERE id = " + to_string(s.pid));
            db.Execute("UPDATE userinfo SET score = score - " + to_string(static_cast<int>(s.price)) + " WHERE username = '" + this->username + "'");
            db.Execute("DELETE FROM buy_records WHERE record_id = " + to_string(s.rid));
            cout << "【退换成功】对应积分已扣除。" << endl;
        }
    }
    catch (...) { cout << "操作失败。" << endl; }
    system("pause");
}

// 3. 我的账户：积分商城逻辑
void Menu::ShowMyselfMenu(Connector& db) {
    while (true) {
        system("cls");
        int currentScore = 0;
        try {
            sql::ResultSet* res = db.Query("SELECT score FROM userinfo WHERE username = '" + this->username + "'");
            if (res && res->next()) currentScore = res->getInt("score");
            delete res;
        }
        catch (...) {}

        cout << "----- 我的账户 (" << this->username << ") -----" << endl;
        cout << "当前可用积分: " << currentScore << " 分" << endl;
        cout << "1. 购买记录\n2. 积分商城\n0. 返回上一级" << endl;

        int choice; cin >> choice;
        if (choice == 0) break;

        if (choice == 1) {
            system("cls");
            cout << "--- 购买历史 ---" << endl;
            sql::ResultSet* res = db.Query("SELECT product_name, buy_price, buy_time FROM buy_records WHERE username = '" + this->username + "' ORDER BY buy_time DESC");
            while (res && res->next()) cout << res->getString("product_name") << "\t" << res->getDouble("buy_price") << "\t" << res->getString("buy_time") << endl;
            delete res; system("pause");
        }
        else if (choice == 2) {
            system("cls");
            cout << "======== 晶东数码积分商城 (当前积分: " << currentScore << ") ========" << endl;
            try {
                sql::ResultSet* res = db.Query("SELECT * FROM points_products WHERE stock > 0");
                cout << left << setw(6) << "ID" << setw(25) << "礼品名称" << "所需积分" << endl;
                while (res && res->next()) {
                    cout << left << setw(6) << res->getInt("id") << setw(25) << res->getString("name") << res->getInt("required_points") << " 分" << endl;
                }
                delete res;

                cout << "\n请输入兑换ID (0返回): ";
                int gid; cin >> gid;
                if (gid == 0) continue;

                sql::ResultSet* ck = db.Query("SELECT * FROM points_products WHERE id = " + to_string(gid));
                if (ck && ck->next()) {
                    int cost = ck->getInt("required_points");
                    if (currentScore >= cost) {
                        db.Execute("UPDATE userinfo SET score = score - " + to_string(cost) + " WHERE username = '" + this->username + "'");
                        db.Execute("UPDATE points_products SET stock = stock - 1 WHERE id = " + to_string(gid));
                        cout << "【兑换成功】数码礼品已发放！" << endl;
                    }
                    else cout << "【兑换失败】积分不足！" << endl;
                }
                delete ck;
            }
            catch (...) { cout << "商城加载异常。" << endl; }
            system("pause");
        }
    }
}