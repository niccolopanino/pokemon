#include "UnexpectedEofException.h"
#include "XmlParserException.h"
#include "../elements/SourcePosition.h"

namespace pkmn::xml
{
    UnexpectedEofException::UnexpectedEofException(int line, const wchar_t* file, const SourcePosition& pos) noexcept
        : XmlParserException(line, file, pos, L"Reached an unexpected end of the file")
    { }
}
