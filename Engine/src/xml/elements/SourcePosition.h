#pragma once
#include <string>

namespace pkmn::xml
{
    struct SourcePosition
    {
        std::wstring m_file;
        size_t m_line = 1u;
        size_t m_col = 1u;
    };
}
