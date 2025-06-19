#include "XmlParserException.h"
#include "../elements/SourcePosition.h"
#include "../../error/BaseException.h"
#include "../../util/StringConv.h"
#include <optional>
#include <ostream>
#include <sstream>
#include <string>

namespace pkmn::xml
{
    XmlParserException::XmlParserException(int line, const wchar_t* file, const SourcePosition& pos,
        const std::optional<std::wstring>& note) noexcept
        : BaseException(line, file), m_src_pos(pos), m_note(note)
    { }

    const wchar_t* XmlParserException::get_type() const noexcept
    {
        return L"XML Parser Exception";
    }

    const SourcePosition& XmlParserException::get_src_pos() const noexcept
    {
        return m_src_pos;
    }

    std::wstring XmlParserException::get_src_pos_string() const noexcept
    {
        std::wostringstream wos;
        wos << L"[XML File] " << m_src_pos.m_file << std::endl
            << L"[at Line:Character] " << std::to_wstring(m_src_pos.m_line)
            << L':' << std::to_wstring(m_src_pos.m_col);
        return wos.str();
    }

    const std::optional<std::wstring>& XmlParserException::get_note() const noexcept
    {
        return m_note;
    }

    const char* XmlParserException::what() const noexcept
    {
        std::wostringstream wos;
        wos << BaseException::what() << std::endl << std::endl
            << get_src_pos_string();

        if (m_note) {
            wos << std::endl << std::endl
                << L"[Note] " << *m_note;
        }

        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
