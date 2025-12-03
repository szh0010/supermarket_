#include "../include/Menu.h"

void Menu::ShowLoginMenu()
{
	// GBK 国标 = “欢迎使用超市系统”
	// UTF-8 
	// 两种不同的编码标准
	std::cout << "----- 欢迎使用晶东超市系统 ------" << std::endl;
	std::cout << "请输入用户名:" << std::endl;
	std::cin >> this->username;
	std::cout << "请输入密码" << std::endl;
	std::cin >> this->password;
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
}
void Menu::ShowShopMenu()
{

}
void Menu::ShowBackMenu()
{

}
void Menu::ShowMyselfMenu()
{

}