#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>


class SettingsParser
{
public:
    SettingsParser(std::string settings_fname);  // old, should be removed
    SettingsParser(std::string settings_fname, std::string task_name);
    std::string get_setting(std::string key);
    void print();
    bool check_setting(std::string key);
    bool success_of_parsing();
private:
    std::string _settings_fname;
    std::string _task_name;
    bool _task_name_set=false;
    std::map<std::string, std::string> _settings;
    // to check whether there is comlete set of settings
    bool _success_of_parsing=false;
    std::vector<std::string> _required_settings_anytime = {
        std::string("Lx"),
        std::string("shape"),
        std::string("disks_number"),
    };
    std::vector<std::string> _required_settings_ternary_polygonal_cylinders = {
        std::string("thickness"),
        std::string("shell_thickness"),
        std::string("outer_radius"),
        std::string("vertices_number"),
    };
    std::map<std::string, std::vector<std::string> >
        _required_settings_specific = {
           std::pair<std::string, std::vector<std::string> >( 
               std::string("ternary_polygonal_cylinders"),
               _required_settings_ternary_polygonal_cylinders
           )
        };
};
