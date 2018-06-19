#include "settings_parser.hpp"


SettingsParser::SettingsParser(std::string fname) {
    _fname = fname;
    std::ifstream fin(_fname);
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
    for (auto it = _settings.begin(); it != _settings.end(); ++it)
        std::cout << "\t" << it->first << " = " << it->second << std::endl;
    std::cout << "--- (from file " << _fname << " ---)\n"
              << "*** End of output ***\n";
}

bool SettingsParser::check_setting(std::string key) {
    return _settings.count(key);
}
