#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>


class SettingsParser
{
public:
    SettingsParser(std::string fname);
    std::string get_setting(std::string key);
    void print();
    bool check_setting(std::string key);
private:
    std::string _fname;
    std::map<std::string, std::string> _settings;
};
