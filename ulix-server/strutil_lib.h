#pragma once
#include <string>
#include <vector>

namespace strutil {
    std::string to_upper(std::string s);
    std::string to_lower(std::string s);
    std::string ltrim(std::string s);
    std::string rtrim(std::string s);
    std::string trim(std::string s);
    std::vector<std::string> split(const std::string& s, char delimiter);
    bool starts_with(const std::string& s, const std::string& prefix);
    bool ends_with(const std::string& s, const std::string& suffix);
}