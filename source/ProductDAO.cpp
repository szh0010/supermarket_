#include "../include/ProductDAO.h"
#include <jdbc/cppconn/prepared_statement.h>

ProductDAO::ProductDAO(Connector& conn)
    : connector(conn) {
}

bool ProductDAO::GetProductAll(vector<Product>& outProducts)
{
    vector<Product> products;
    try {
        auto connPtr = connector.GetConnection();
        // 建议加上 ORDER BY id ASC 确保顺序
        std::unique_ptr<sql::PreparedStatement> stmt(
            connPtr->prepareStatement("SELECT id, product_name, price, stock FROM products ORDER BY id ASC")
        );

        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        int count = 0;
        // 直接进入 while 循环，不要在循环外面写 res->next()
        while (res->next())
        {
            // 提取数据
            int id = res->getInt("id");
            std::string name = res->getString("product_name");
            double price = res->getDouble("price");
            int stock = res->getInt("stock");

            // 通过构造函数添加
            products.emplace_back(id, name, price, stock);
            count++;
        }

        if (count == 0) {
            std::cout << "[DAO] 数据库中没有商品记录。" << std::endl;
            return false;
        }

        std::cout << "查询到的商品数量: " << count << std::endl;
        outProducts = products;
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "GetProductAll SQLException: " << e.what() << std::endl;
        return false;
    }
}