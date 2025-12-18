#pragma once
#include "Connector.h"
#include "ProductService.h"
#include <iostream>
#include <string>

class Menu
{
// 公有的
public:
	//Menu() : username(""), password(""), userType(0) {}  // 初始化成员变量
	void ShowLoginMenu();
	void ShowAdminMenu(); // 自己去完善
	void ShowGuestMenu(ProductService productService); // 自己去完善
	int GetUserType(); // 返回用户类型
	std::string GetUsername();
	std::string GetPassword();
	std::string GetGender();


	void SetUserType(int userType);
	void ShowOpenMenu();
	void ShowGiveMenu();
	void ShowPassMenu();
	void ShowShopMenu(const std::vector<Product>& products);
	void ShowBackMenu();
	void ShowMyselfMenu();
	void ShowEnterMenu();
	void ShowRegisterMenu();

	void SetGender(const std::string gender);

// 私有的
private:
	std::string username;
	std::string password;
	std::string gender;
	Connector connector;
	int userType; // 0是管理，1是顾客 
};