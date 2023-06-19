#include "Util.h"

namespace Util {
    std::vector<std::string> string_split(const std::string& text, char sep) {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;

        while ((end = text.find(sep, start)) != std::string::npos) {
            if (text.substr(start, end - start) != "") tokens.push_back(text.substr(start, end - start));

            start = end + 1;
        }

        if (text.substr(start) != "") tokens.push_back(text.substr(start));

        return tokens;
    }

    bool contains(std::vector<RE::TESForm*> haystack, RE::TESForm* needle) {
        return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
    }
}