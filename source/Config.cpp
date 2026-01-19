#include "../include/Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <locale>

Config& Config::Instance()
{
    static Config instance;
    return instance;
}

void Config::Load(const std::string& file)
{
    std::ifstream fin(file);
    if (!fin.is_open()) {
        std::cerr << "无法打开配置文件: " << file << std::endl;
        return;
    }

    fin.imbue(std::locale("en_US.UTF-8"));  // 设置为 UTF-8 编码
    std::string line;
    while (std::getline(fin, line)) {

        // 跳过空行和注释
        if (line.empty() || line[0] == '#')
            continue;

        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos)
            continue;

        std::string key = line.substr(0, equalPos);
        std::string value = line.substr(equalPos + 1);

        // 去除键和值的前后空格
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        data[key] = value;
    }
}

std::string Config::Get(const std::string& key)
{
    if (data.count(key))
        return data[key];
    else {
        std::cerr << "配置文件中缺少键: " << key << std::endl;
        return "";
    }
}
