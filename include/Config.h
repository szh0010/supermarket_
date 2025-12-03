#pragma once
#include <string>
#include <map>

class Config
{
public:
    static Config& Instance();
    void Load(const std::string& file);
    std::string Get(const std::string& key);

private:
    Config() = default;
    std::map<std::string, std::string> data;
};
