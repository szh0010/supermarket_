#pragma once
#include "ProductDAO.h"

// 业务逻辑层
// Service = 服务
class ProductService {
public:
    ProductService(ProductDAO& dao);
    bool GetAllProducts(vector<Product>& outProducts);
    void PrintProducts(vector<Product> outProducts);

private:
    ProductDAO& dao;
};