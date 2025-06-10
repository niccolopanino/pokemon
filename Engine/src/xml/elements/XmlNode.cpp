#include "SourcePosition.h"
#include "XmlNode.h"
#include <memory>
#include <string>

namespace pkmn::xml::elem
{
    XmlNode::XmlNode(SourcePosition pos, std::shared_ptr<XmlNode> parent) noexcept
        : m_pos(pos), m_parent(parent)
    { }

    const SourcePosition& XmlNode::get_pos() const noexcept
    {
        return m_pos;
    }

    std::shared_ptr<XmlNode> XmlNode::get_parent() noexcept
    {
        return m_parent;
    }
}
