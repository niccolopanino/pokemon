#include "SourcePosition.h"
#include "XmlComment.h"
#include "XmlMisc.h"
#include "XmlNode.h"
#include <memory>
#include <sstream>
#include <string>

namespace pkmn::xml
{
    XmlComment::XmlComment(SourcePosition pos, std::wstring comment,
        std::shared_ptr<XmlNode> parent) noexcept
        : XmlMisc(pos, parent), m_comment(comment)
    { }

    const std::wstring& XmlComment::get_comment() const noexcept
    {
        return m_comment;
    }

    std::wstring XmlComment::to_wstring(size_t indent_level, size_t indent_size) const noexcept
    {
        std::wostringstream wos;
        wos << std::wstring(indent_level * indent_size, L' ') << L"<!-- " << m_comment << L" -->";
        return wos.str();
    }
}
