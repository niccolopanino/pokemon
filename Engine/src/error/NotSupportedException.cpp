#include "BaseException.h"
#include "NotSupportedException.h"
#include "../util/StringConv.h"
#include <ostream>
#include <string>
#include <sstream>

namespace pkmn
{
    NotSupportedException::NotSupportedException(int line, const wchar_t* file, std::wstring feature) noexcept
        : BaseException(line, file), m_feature(feature)
    { }

    const std::wstring& NotSupportedException::get_feature() const noexcept
    {
        return m_feature;
    }

    const wchar_t* NotSupportedException::get_type() const noexcept
    {
        return L"Pokémon Feature Not Supported Exception";
    }

    const char* NotSupportedException::what() const noexcept
    {
        std::wostringstream wos;
        wos << BaseException::what() << std::endl
            << L"[Feature] " << get_feature() << L" is not supported (yet)";
        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
