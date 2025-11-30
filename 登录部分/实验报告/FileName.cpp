#include<iostream>
#include<string>
#include<iomanip>
using namespace std;
int main()
{
	system("mode con cols=40 lines=30");
	system("color F0");
	system("title ÕËºÅµÇÂ¼");
	long long zhanghao;
	string mima;
	cout << setw(12) << "" << "ÓÃ»§ÕËºÅ£º";
	cout << "\n" << setw(12) << "" << "ÃÜÂë£º";
	cout << "\x1B[1A";
	cout << "\x1B[6C";
	cin >> zhanghao;
	cout << "\x1B[1B";
	cout << "\x1B[6C";
	cin >> mima;
	system("pause>nul");
	return 0;
}