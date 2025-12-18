// Product.h
#include<iostream>
#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>

class Product {
public:


	// 构造函数
	Product(int id, const std::string& name, double price, int stock)
		: id(id), name(name), price(price), stock(stock)
	{
	}
	
	// 打印商品信息，业务逻辑
	void Print() const
	{
		std::cout << "ID: " << id << ", Name: " << name<< ", Price: " << price << ", Stock: " << stock << std::endl;
	}
private:
	int id;             // 商品ID
	std::string name;   // 商品名称
	double price;       // 商品价格
	int stock;          // 库存
};

#endif // PRODUCT_H