#pragma once
#include <string>

namespace pkmn::xml
{
    struct XmlAttribute
    {
        std::wstring m_name;
        std::wstring m_value;

        std::wstring to_wstring() const noexcept;
    };
}
