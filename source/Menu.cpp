#include "../include/Menu.h"
#include "../include/Product.h"
#include "../include/Connector.h"
#include <vector>
#include <iomanip> // 用于格式化输出

void Menu::ShowEnterMenu()
{
    std::cout << "----- 欢迎使用晶东超市系统 ------" << std::endl;
    std::cout << "1.老用户登录" << std::endl << "2.新用户注册" << std::endl;
}

void Menu::ShowLoginMenu()
{
    std::cout << "----- 欢迎使用晶东超市系统 ------" << std::endl;
    std::cout << "请输入用户名:" << std::endl;
    std::cin >> this->username;
    std::cout << "请输入密码:" << std::endl;
    std::cin >> this->password;
    system("cls");
}

void Menu::ShowRegisterMenu()
{
    std::cout << "----- 欢迎使用注册系统 ------" << std::endl;
    std::cout << "请输入用户名:" << std::endl;
    std::cin >> this->username;
    std::cout << "请输入密码:" << std::endl;
    std::cin >> this->password;
    std::cout << "请选择你的性别:" << std::endl;
    std::cout << "1. 男性" << std::endl;
    std::cout << "2. 女性" << std::endl;
    int choice = 0;
    std::cin >> choice;
    if (choice == 1) SetGender("male");
    else if (choice == 2) SetGender("female");
    else SetGender("other");
    system("cls");
}

void Menu::SetGender(const std::string gender) { this->gender = gender; }
int Menu::GetUserType() { return this->userType; }
std::string Menu::GetUsername() { return this->username; }
std::string Menu::GetPassword() { return this->password; }
std::string Menu::GetGender() { return this->gender; }
void Menu::SetUserType(int userType) { this->userType = userType; }

void Menu::ShowAdminMenu()
{
    std::cout << "----- 欢迎使用晶东超市管理系统 ------" << std::endl;
    std::cout << "----- 1.查看超市经营情况 ------------" << std::endl;
    std::cout << "----- 2.货物补充 --------------------" << std::endl;
    std::cout << "----- 3.过期货物处理 ----------------" << std::endl;
}

void Menu::ShowOpenMenu()
{
    std::cout << "本日营收情况：" << std::endl;
    std::cout << "历史营收情况：" << std::endl;
}

void Menu::ShowGiveMenu() { std::cout << "目前货物剩余量：" << std::endl; }
void Menu::ShowPassMenu() { std::cout << "目前临期货物陈列：" << std::endl; }

// 修改 1: 增加 Connector& db 参数
void Menu::ShowGuestMenu(ProductService productService, Connector& db)
{
    while (true) {
        system("cls");
        std::cout << "----- 欢迎光临晶东超市 (" << this->username << ") ------" << std::endl;
        std::cout << "----- 1.选购商品 ------------" << std::endl;
        std::cout << "----- 2.退换商品 ------------" << std::endl;
        std::cout << "----- 3.我的账户 ------------" << std::endl;
        std::cout << "----- 0.退出登录 ------------" << std::endl;

        int num;
        std::cin >> num;
        if (num == 1) {
            std::vector<Product> products;
            productService.GetAllProducts(products);
            ShowShopMenu(products, db); // 传递 db
        }
        else if (num == 2) {
            ShowBackMenu();
        }
        else if (num == 3) {
            ShowMyselfMenu(db); // 传递 db
        }
        else if (num == 0) {
            break;
        }
    }
}

// 修改 2: 实现购买即存入数据库
void Menu::ShowShopMenu(const std::vector<Product>& products, Connector& db)
{
    system("cls");
    std::cout << "----- 商品列表 -----" << std::endl;
    if (products.empty()) {
        std::cout << "没有可用商品。" << std::endl;
        return;
    }

    for (const auto& product : products) {
        product.Print();
    }

    std::cout << "\n请输入商品编号购买 (输入0返回)：" << std::endl;
    int productId;
    std::cin >> productId;
    if (productId == 0) return;

    bool found = false;
    for (const auto& product : products)
    {
        if (product.GetId() == productId)
        {
            found = true;
            if (product.GetStock() > 0)
            {
                try {
                    // 1. 更新库存 (UPDATE)
                    string updateSql = "UPDATE products SET stock = stock - 1 WHERE id = " + std::to_string(productId);
                    db.Execute(updateSql);

                    // 2. 存入购买记录 (INSERT) 到 buy_records 表
                    string insertSql = "INSERT INTO buy_records(username, product_id, product_name, buy_price) VALUES (?, ?, ?, ?)";
                    auto pstmt = db.Prepare(insertSql);
                    pstmt->setString(1, this->username);
                    pstmt->setInt(2, product.GetId());
                    pstmt->setString(3, product.GetName());
                    pstmt->setDouble(4, product.GetPrice()); // 确保 Product.h 有 GetPrice() 方法

                    pstmt->executeUpdate();

                    std::cout << "【购买成功】您已购买: " << product.GetName() << "，记录已同步到您的账户。" << std::endl;
                }
                catch (sql::SQLException& e) {
                    std::cerr << "【数据库错误】购买失败: " << e.what() << std::endl;
                }
            }
            else {
                std::cout << "【购买失败】商品 \"" << product.GetName() << "\" 库存为0！" << std::endl;
            }
            break;
        }
    }

    if (!found) std::cout << "未找到该编号的商品。" << std::endl;
    system("pause");
}

void Menu::ShowBackMenu() { std::cout << "请选择要退还的商品：" << std::endl; system("pause"); }

// 修改 3: 实现进入账户后查询记录
void Menu::ShowMyselfMenu(Connector& db)
{
    system("cls");
    std::cout << "----- 我的账户 (" << this->username << ") -----" << std::endl;
    std::cout << "----- 1.购买记录 ------------" << std::endl;
    std::cout << "----- 2.我的积分 ------------" << std::endl;
    std::cout << "----- 3.积分商场 ------------" << std::endl;
    std::cout << "----- 0.返回上一级 ----------" << std::endl;

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "\n正在加载您的历史记录..." << std::endl;
        try {
            // 查询 buy_records 表
            string sql = "SELECT product_name, buy_price, buy_time FROM buy_records WHERE username = '" + this->username + "' ORDER BY buy_time DESC";
            sql::ResultSet* res = db.Query(sql);

            if (!res || res->rowsCount() == 0) {
                std::cout << "您目前还没有任何购买记录。" << std::endl;
            }
            else {
                std::cout << std::left << std::setw(15) << "商品名称" << std::setw(10) << "价格" << "购买时间" << std::endl;
                std::cout << "--------------------------------------------------------" << std::endl;
                while (res->next()) {
                    std::cout << std::left << std::setw(15) << res->getString("product_name")
                        << std::setw(10) << res->getDouble("buy_price")
                        << res->getString("buy_time") << std::endl;
                }
            }
            delete res; // 必须手动释放 ResultSet 指针
        }
        catch (sql::SQLException& e) {
            std::cout << "查询记录失败: " << e.what() << std::endl;
        }
        system("pause");
    }
}