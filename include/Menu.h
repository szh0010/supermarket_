#pragma once
#include <iostream>
#include <string>

class Menu
{
// 公有的
public:
	void ShowLoginMenu();
	void ShowAdminMenu(); // 自己去完善
	void ShowGuestMenu(); // 自己去完善
	int GetUserType(); // 返回用户类型
	std::string GetUsername();
	std::string GetPassword();
	void SetUserType(int userType);
	void ShowOpenMenu();
	void ShowGiveMenu();
	void ShowPassMenu();
	void ShowShopMenu();
	void ShowBackMenu();
	void ShowMyselfMenu();
// 私有的
private:
	std::string username;
	std::string password;
	int userType; // 0是管理，1是顾客 
};