#include "../include/ProductService.h"

// ÒµÎñÂß¼­²ã
ProductService::ProductService(ProductDAO& dao)
    : dao(dao) {
}

bool ProductService::GetAllProducts(vector<Product>& outProducts)
{
    if (dao.GetProductAll(outProducts))
    {
        return true;
    }
    return false;
}

void ProductService::PrintProducts(vector<Product> outProducts)
{
    for (int i = 0; i < outProducts.size(); i++)
    {
        outProducts[i].Print();
    }
}