#include "StringConv.h"

#include <cstdlib>

namespace pkmn
{
    std::wstring str_to_wstr(const std::string &narrow)
    {
        size_t conv_chars = 0;
        wchar_t *wide = new wchar_t[narrow.size() + 1];
        mbstowcs_s(&conv_chars, wide, narrow.size() + 1, narrow.c_str(), _TRUNCATE);
        std::wstring retval(wide);
        delete[] wide;
        return retval;
    }

    std::string wstr_to_str(const std::wstring &wide)
    {
        size_t conv_chars = 0;
        char *narrow = new char[wide.size() + 1];
        wcstombs_s(&conv_chars, narrow, wide.size() + 1, wide.c_str(), _TRUNCATE);
        std::string retval(narrow);
        delete[] narrow;
        return retval;
    }
}
