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
        std::unique_ptr<sql::PreparedStatement> stmt(
            connPtr->prepareStatement("SELECT * FROM products")
        );
        
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        if (!res->next()) {
            return false;
        }

        int count = 0;
        while (res->next())
        {
            Product temp(res->getInt("id"),
                res->getString("product_name"),
                res->getDouble("price"), 0);

            products.emplace_back(temp);
            count++;
        }
        std::cout << "查询到的商品数量: " << count << std::endl; // 打印商品数量
        outProducts = products; // products我们从数据库中查到的值，把这个全部的商品信息赋值给outProducts
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "GetProductAll SQLException: " << e.what() << std::endl;
        return false;
    }
}