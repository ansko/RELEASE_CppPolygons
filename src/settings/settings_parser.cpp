#include "settings_parser.hpp"


SettingsParser::SettingsParser(std::string settings_fname, std::string task_name) {
    _settings_fname = settings_fname;
    _task_name = task_name;
    _task_name_set = true;
    std::ifstream fin(_settings_fname);
    std::string key;
    std::string value;
    while(fin >> key && fin >> value) {
       // _settings.insert(std::pair<std::string, std::string>(
       //     std::make_pair(key, value)));
       _settings[key] = value;
    }
    fin.close();

    for (auto setting_out : _settings)
        std::cout << setting_out.first << " " << setting_out.second << "\n";

    for (auto required : _required_settings_anytime) {
        if (!check_setting(required)) {
            std::cout << "Required setting is not set (anytime): "
                      << required << std::endl;
            return;
        }
    }
    if (_required_settings_specific.count(task_name) != 0)
        for (auto required : _required_settings_specific[task_name]) {
            if (!check_setting(required)) {
                std::cout << "Required setting is not set: (specific)"
                          << required << std::endl;
                return;
            }
        }
     else
        std::cout << "Warning: specific settings for this task are not set\n";
    _success_of_parsing = true;
    return;
}

// should be removed!
SettingsParser::SettingsParser(std::string settings_fname) {
    _settings_fname = settings_fname;
    std::ifstream fin(_settings_fname);
    std::string key;
    std::string value;
    while(fin >> key && fin >> value) {
        _settings.insert(std::pair<std::string, std::string>(
            std::make_pair(key, value)));
    }
    fin.close();

    return;
}

std::string SettingsParser::get_setting(std::string key) {
    return _settings[key];
}

void SettingsParser::print() {
    std::cout << "*** All parsed settings ***\n";
    if (_task_name_set)
        std::cout << "--- task_name_set to " << _task_name << " ---\n";
    else
        std::cout << "--- task_name_set: no---\n";
    for (auto it = _settings.begin(); it != _settings.end(); ++it)
        std::cout << "\t" << it->first << " = " << it->second << std::endl;
    std::cout << "--- from file " << _settings_fname << " ---\n"
              << "--- for task " <<  _task_name << "---\n"
              << "--- with success_state: " << _success_of_parsing << " ---\n"
              << "*** End of output ***\n";
}

bool SettingsParser::check_setting(std::string key) {
    return _settings.count(key);
}

bool SettingsParser::success_of_parsing() {
    return _success_of_parsing;
}
