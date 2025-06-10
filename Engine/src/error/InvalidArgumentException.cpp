#include "BaseException.h"
#include "InvalidArgumentException.h"
#include "../util/StringConv.h"
#include <optional>
#include <ostream>
#include <sstream>
#include <string>

namespace pkmn
{
    InvalidArgumentException::InvalidArgumentException(int line, const wchar_t* file,
        const std::wstring& note, std::optional<std::wstring> argname) noexcept
        : BaseException(line, file), m_note(note), m_argname(argname)
    { }

    const wchar_t* InvalidArgumentException::get_type() const noexcept
    {
        return L"Pokémon Invalid Argument Exception";
    }

    const std::wstring& InvalidArgumentException::get_note() const noexcept
    {
        return m_note;
    }

    const char* InvalidArgumentException::what() const noexcept
    {
        std::wostringstream wos;
        wos << BaseException::what() << std::endl
            << L"[Note] " << get_note();
        if (m_argname)
            wos << std::endl << L"[Argument Name] " << *m_argname;
        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
