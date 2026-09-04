#include"strutil_lib.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace strutil {

    // 1. ѕеревод строки в верхний регистр
    std::string to_upper(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }

    // 2. ѕеревод строки в нижний регистр
    std::string to_lower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    // 3. ”даление пробелов в начале строки (LTrim)
    std::string ltrim(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
        return s;
    }

    // 4. ”даление пробелов в конце строки (RTrim)
    std::string rtrim(std::string s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
        return s;
    }

    // 5. ”даление пробелов с обоих концов (Trim)
    std::string trim(std::string s) {
        return ltrim(rtrim(s));
    }

    // 6. –азделение строки по разделителю (Split)
    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // 7. ѕроверка: начинаетс€ ли строка с определенной подстроки
    bool starts_with(const std::string& s, const std::string& prefix) {
        return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
    }

    // 8. ѕроверка: заканчиваетс€ ли строка определенной подстрокой
    bool ends_with(const std::string& s, const std::string& suffix) {
        return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
}
