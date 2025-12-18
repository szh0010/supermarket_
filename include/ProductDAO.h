#pragma once
#include <string>
#include <memory>
#include "Product.h"
#include "Connector.h"

class ProductDAO {
public:
    ProductDAO(Connector& conn);

    bool GetProductAll(vector<Product>& outProducts);
    bool GetProductByName(std::string product_name, Product& outProduct);
    bool GetProductById();

private:
    Connector& connector;
};