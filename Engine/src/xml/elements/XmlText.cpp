#include "SourcePosition.h"
#include "XmlNode.h"
#include "XmlText.h"
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace pkmn::xml
{
    XmlText::XmlText(SourcePosition pos, std::wstring&& text, std::shared_ptr<XmlNode> parent) noexcept
        : XmlNode(pos, parent), m_text(std::move(text))
    { }

    const std::wstring& XmlText::get_text() const noexcept
    {
        return m_text;
    }

    std::wstring XmlText::to_wstring(size_t indent_level, size_t indent_size) const noexcept
    {
        std::wstring line;
        std::vector<std::wstring> lines;
        std::wstringstream wss(m_text);
        while (std::getline(wss, line, L'\n'))
            lines.push_back(line);

        std::wostringstream wos;
        wos << std::wstring(indent_level * indent_size, L' ') << lines[0u];
        for (size_t i = 1u; i < lines.size(); ++i)
            wos << std::endl << std::wstring(indent_level * indent_size, L' ') << lines[i];
        return wos.str();
    }
}
