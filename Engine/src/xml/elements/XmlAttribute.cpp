#include "XmlAttribute.h"
#include <ostream>
#include <sstream>
#include <string>

namespace pkmn::xml
{
    std::wstring XmlAttribute::to_wstring() const noexcept
    {
        std::wostringstream wos;
        wos << m_name << L"=\"" << m_value << L"\"";
        return wos.str();
    }
}
