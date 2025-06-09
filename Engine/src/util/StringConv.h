#pragma once
#include <string>

namespace pkmn
{
    // converts a narrow string to a wide string
    std::wstring str_to_wstr(const std::string &narrow);
    // converts a wide string to a narrow string
    std::string wstr_to_str(const std::wstring &wide);
}
