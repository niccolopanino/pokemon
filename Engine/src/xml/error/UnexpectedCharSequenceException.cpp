#include "UnexpectedCharSequenceException.h"
#include "XmlParserException.h"
#include "../elements/SourcePosition.h"
#include "../../util/StringConv.h"
#include <ostream>
#include <string>
#include <sstream>

namespace pkmn::xml
{
    UnexpectedCharSequenceException::UnexpectedCharSequenceException(int line, const wchar_t* file,
        const SourcePosition& pos, const std::wstring& expected, const std::wstring& actual) noexcept
        : XmlParserException(line, file, pos), m_expected(expected), m_actual(actual)
    { }

    const wchar_t* UnexpectedCharSequenceException::get_type() const noexcept
    {
        return L"XML Parser Unexpected Character Sequence Exception";
    }

    const std::wstring& UnexpectedCharSequenceException::get_expected_string() const noexcept
    {
        return m_expected;
    }

    const std::wstring& UnexpectedCharSequenceException::get_actual_string() const noexcept
    {
        return m_actual;
    }

    const char* UnexpectedCharSequenceException::what() const noexcept
    {
        std::wostringstream wos;
        wos << get_type() << std::endl
            << get_src_pos_string() << std::endl
            << L"[Expected] " << m_expected << std::endl
            << L"[Got] " << m_actual;

        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
