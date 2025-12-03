#include "../include/Connector.h"
#include "../include/Config.h"
// [yyx] - 12.3重构
//初始化构造函数
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
		conn = std::unique_ptr<sql::Connection>(driver->connect(host, user, password));
		conn->setSchema(database);
		std::cout << "[DB] 数据库连接成功！" << std::endl;
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "[DB Error] 连接失败: " << e.what() << std::endl;
		std::cerr << "MySQL Code: " << e.getErrorCode()
			<< "  SQLState: " << e.getSQLState() << std::endl;
		return false;
	}
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
