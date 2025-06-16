#pragma once
#include "SourcePosition.h"
#include <memory>
#include <string>

namespace pkmn::xml
{
    class XmlNode
    {
    public:
        virtual ~XmlNode() = default;
    protected:
        XmlNode(SourcePosition pos, std::shared_ptr<XmlNode> parent) noexcept;
    public:
        const SourcePosition& get_pos() const noexcept;
        std::shared_ptr<XmlNode> get_parent() noexcept;
        virtual std::wstring to_wstring(size_t indent_level = 0u,
            size_t indent_size = 4u) const noexcept = 0;
    protected:
        SourcePosition m_src_pos;
        std::shared_ptr<XmlNode> m_parent;
    };
}
