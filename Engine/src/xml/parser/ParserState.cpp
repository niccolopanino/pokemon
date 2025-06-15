#include "ParserState.h"
#include <string>

namespace pkmn::xml
{
    std::wstring parser_state_to_wstring(ParserState state) noexcept
    {
        switch (state)
        {
        case pkmn::xml::ParserState::e_null:
            return L"e_null";
        case pkmn::xml::ParserState::e_ang_o:
            return L"e_ang_o";
        case pkmn::xml::ParserState::e_ang_o_qm:
            return L"e_ang_o_qm";
        case pkmn::xml::ParserState::e_ang_o_em:
            return L"e_ang_o_em";
        case pkmn::xml::ParserState::e_tag:
            return L"e_tag";
        case pkmn::xml::ParserState::e_elem:
            return L"e_elem";
        case pkmn::xml::ParserState::e_eof:
            return L"e_eof";
        default:
            return L"";
        }
    }
}
