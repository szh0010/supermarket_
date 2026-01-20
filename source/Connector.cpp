#include "../include/Connector.h"
#include "../include/Config.h"
#include "../include/Product.h"
#include <vector>

// [yyx] - 12.3重构
// 初始化构造函数
Connector::Connector()
{
	LoadConfig();
	Connect();
}

Connector::~Connector()
{
	if (conn)
	{
		conn->close();
	}
}

void Connector::LoadConfig()
{
	Config& cfg = Config::Instance();

	host = cfg.Get("database.host");
	user = cfg.Get("database.user");
	password = cfg.Get("database.password");
	database = cfg.Get("database.name");
}

bool Connector::Connect()
{
	try {
		driver = sql::mysql::get_mysql_driver_instance();
		// 注意：此处 conn 需为 shared_ptr 确保兼容其他方法
		conn = std::shared_ptr<sql::Connection>(driver->connect(host, user, password));
		conn->setSchema(database);

		// --- 核心修改开始 ---
		// 显式通过 SQL 命令设置字符集，确保双向传输一致
		std::unique_ptr<sql::Statement> stmt(conn->createStatement());
		stmt->execute("SET NAMES utf8mb4");
		// --- 核心修改结束 ---

		std::cout << "[DB] 数据库连接成功！" << std::endl;
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "[DB Error] 连接失败: " << e.what() << std::endl;
		return false;
	}
}

// --- 新增：获取商品列表的核心逻辑 ---
std::vector<Product> Connector::GetProducts() {
	std::vector<Product> products;
	try {
		if (!conn || conn->isClosed()) {
			Connect();
		}

		// 1. 创建 Statement，生命周期覆盖整个读取过程
		std::unique_ptr<sql::Statement> stmt(conn->createStatement());

		// 2. 确保使用了 ORDER BY id ASC 排序，防止 ID 乱序或找不到大米
		std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT id, product_name, price, stock FROM products ORDER BY id ASC"));

		while (res->next()) {
			// 3. 从结果集中读取数据
			int id = res->getInt("id");
			std::string name = res->getString("product_name");
			double price = res->getDouble("price");
			int stock = res->getInt("stock");

			// 4. 利用 emplace_back 调用 Product 的构造函数
			// 这样可以直接初始化 private 成员变量
			products.emplace_back(id, name, price, stock);
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "[DB Error] 获取商品失败: " << e.what() << std::endl;
	}
	return products;
}

int Connector::Execute(const std::string& sql)
{
	try {
		std::unique_ptr<sql::Statement> stmt(conn->createStatement());
		return stmt->executeUpdate(sql);
	}
	catch (sql::SQLException& e) {
		std::cerr << "[DB Error] 执行失败: " << e.what() << std::endl;
		return -1;
	}
}

std::unique_ptr<sql::PreparedStatement> Connector::Prepare(const std::string& sql)
{
	return std::unique_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
}

std::shared_ptr<sql::Connection> Connector::GetConnection() {
	try {
		if (!conn || conn->isClosed()) {
			std::cout << "[DB] 连接断开，尝试自动重连..." << std::endl;
			Connect();
		}
	}
	catch (...) {
		Connect();
	}

	return conn;
}

// 在 Connector.cpp 中定义 Query 方法
sql::ResultSet* Connector::Query(const std::string& sql) {
	try {
		sql::Statement* stmt = conn->createStatement();  // 注意：手动模式
		return stmt->executeQuery(sql);
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what() << std::endl;
		return nullptr;
	}
}

sql::ResultSet* Connector::GetResultPointer(const sql::SQLString& query)
{
	try {
		sql::Statement* stmt = conn->createStatement();
		return stmt->executeQuery(query);
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what() << std::endl;
		return nullptr;
	}
}