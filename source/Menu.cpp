#include "../include/Menu.h"
#include "../include/Product.h"
#include "../include/Connector.h"
#include<vector>
void Menu::ShowEnterMenu()
{
	std::cout << "----- 欢迎使用晶东超市系统 ------" << std::endl;
	std::cout << "1.老用户登录" << std::endl << "2.新用户注册" << std::endl;
}

void Menu::ShowLoginMenu()
{
	// GBK 国标 = “欢迎使用超市系统”
	// UTF-8 
	// 两种不同的编码标准
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
	if (choice == 1)
	{
		SetGender("male");
	}
	else if (choice == 2)
	{
		SetGender("female");
	}
	else
	{
		SetGender("other");
	}
	system("cls");
}

void Menu::SetGender(const std::string gender)
{
	this->gender = gender;
}

int Menu::GetUserType()
{
	return this->userType;
}

std::string Menu::GetUsername()
{
	return this->username;
}

std::string Menu::GetPassword()
{
	return this->password;
}

std::string Menu::GetGender()
{
	return this->gender;
}

void Menu::SetUserType(int userType)
{
	this->userType = userType;
}

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
void Menu::ShowGiveMenu()
{
	std::cout << "目前货物剩余量：" << std::endl;
}
void Menu::ShowPassMenu()
{
	std::cout << "目前临期货物陈列：" << std::endl;
}
void Menu::ShowGuestMenu()
{
	std::cout << "----- 欢迎光临晶东超市 ------" << std::endl;
	std::cout << "----- 1.选购商品 ------------" << std::endl;
	std::cout << "----- 2.退换商品 ------------" << std::endl;
	std::cout << "----- 3.我的账户 ------------" << std::endl;
	int num;
	std::cin >> num;
	if (num == 1)
	{
		std::vector<Product> products = connector.GetProducts();
		ShowShopMenu(products);
	}
}
void Menu::ShowShopMenu(const std::vector<Product>& products)
{
	system("cls");
	std::cout << "----- 商品列表 -----" << std::endl;
	if (products.empty()) 
	{
		std::cout << "没有可用商品。" << std::endl;
		return;
	}

	// 显示商品列表
	for (const auto& product : products) 
	{
		product.Print();  // 使用 Product 类中的 Print 方法显示商品信息
	}

	std::cout << "请选择商品编号购买：" << std::endl;
	int productId;
	std::cin >> productId;
}
void Menu::ShowBackMenu()
{

}
void Menu::ShowMyselfMenu()
{

}