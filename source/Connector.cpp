#include "../include/Connector.h"
//初始化构造函数
Connector::Connector() 
{
	driver = sql::mysql::get_mysql_driver_instance();
	conn = std::unique_ptr<sql::Connection>(driver->connect("tcp://127.0.0.1:3306", "root", "123456"));
	conn->setSchema("db");
	std::cout << "Connector初始化成功" << std::endl;
}

sql::ResultSet* Connector::GetResultPointer(const sql::SQLString& sql)
{
	try {
		std::unique_ptr<sql::Statement> stmt(conn->createStatement());
		sql::ResultSet* res = stmt->executeQuery(sql);  // 返回普通指针
		return res;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what() << std::endl;
		return nullptr;
	}
}

void Connector::TestShow(sql::ResultSet* res)
{
	// 测试一下输出结果
	while (res->next()) {
		std::cout << "Username: " << res->getString("username") << std::endl;
		std::cout << "-----------------------------" << std::endl;
	}

}