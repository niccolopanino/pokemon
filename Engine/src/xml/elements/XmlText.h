#pragma once
#include "SourcePosition.h"
#include "XmlNode.h"
#include <memory>
#include <string>

namespace pkmn::xml
{
    class XmlText : public XmlNode
    {
    public:
        XmlText(SourcePosition pos, std::wstring&& text, std::shared_ptr<XmlNode> parent) noexcept;
    public:
        const std::wstring& get_text() const noexcept;
        virtual std::wstring to_wstring(size_t indent_level = 0u, size_t indent_size = 4u) const noexcept override;
    private:
        std::wstring m_text;
    };
}
