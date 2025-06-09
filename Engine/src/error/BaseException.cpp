#include "BaseException.h"
#include "../util/StringConv.h"
#include <sstream>

namespace pkmn
{
    BaseException::BaseException(int line, const wchar_t* file) noexcept
        : m_line(line), m_file(file)
    { }

    const wchar_t* BaseException::get_type() const noexcept
    {
        return L"Pokémon Base Exception";
    }

    int BaseException::get_line() const noexcept
    {
        return m_line;
    }

    const std::wstring& BaseException::get_file() const noexcept
    {
        return m_file;
    }

    std::wstring BaseException::get_origin_string() const noexcept
    {
        std::wostringstream wos;
        wos << L"[File] " << m_file << std::endl
            << L"[Line] " << m_line;
        return wos.str();
    }

    const char* BaseException::what() const noexcept
    {
        std::wostringstream wos;
        wos << get_type() << std::endl
            << get_origin_string();
        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
