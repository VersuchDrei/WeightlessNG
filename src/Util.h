#pragma once

namespace Util {
    std::vector<std::string> string_split(const std::string& text, char sep);
    bool contains(std::vector<RE::TESForm*> haystack, RE::TESForm* needle);
}