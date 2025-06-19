#include "IllegalParserStateException.h"
#include "XmlParserException.h"
#include "../elements/SourcePosition.h"
#include "../parser/ParserState.h"
#include "../../util/StringConv.h"
#include <ostream>
#include <sstream>

namespace pkmn::xml
{
    IllegalParserStateException::IllegalParserStateException(int line, const wchar_t* file,
        const SourcePosition& pos, ParserState state) noexcept
        : XmlParserException(line, file, pos), m_state(state)
    { }

    const wchar_t* IllegalParserStateException::get_type() const noexcept
    {
        return L"XML Illegal Parser State Exception";
    }

    ParserState IllegalParserStateException::get_state() const noexcept
    {
        return m_state;
    }

    const char* IllegalParserStateException::what() const noexcept
    {
        std::wostringstream wos;
        wos << XmlParserException::what() << std::endl
            << L"[Parser State] " << parser_state_to_wstring(m_state);

        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
