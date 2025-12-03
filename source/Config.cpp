#include "../include/Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

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

        data[key] = value;
    }
}

std::string Config::Get(const std::string& key)
{
    if (data.count(key))
        return data[key];
    return "";
}
