#pragma once
#include <string>

namespace pkmn::xml
{
    enum class ParserState
    {
        e_null, // initial state or currently parsing at document's top-level
        e_ang_o, // last read an opening angular bracket
        e_ang_o_qm, // last read '<?'
        e_ang_o_em, // last read '<!'
        e_tag, // read identifier of start tag or empty element tag, processing rest of tag
        e_elem, // currently inside of an element
        e_eof // reached the end of the file
    };

    std::wstring parser_state_to_wstring(ParserState state) noexcept;
}
