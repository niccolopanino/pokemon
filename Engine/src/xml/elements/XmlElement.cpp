#include "SourcePosition.h"
#include "XmlAttribute.h"
#include "XmlElement.h"
#include "XmlNode.h"
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

namespace pkmn::xml
{
    XmlElement::XmlElement(SourcePosition pos, std::wstring&& tag,
        std::shared_ptr<XmlNode> parent) noexcept
        : XmlNode(pos, parent), m_tagname(std::move(tag))
    { }

    const std::wstring& XmlElement::get_name() const noexcept
    {
        return m_tagname;
    }

    const std::vector<XmlAttribute>& XmlElement::get_attributes() const noexcept
    {
        return m_attributes;
    }

    std::optional<std::wstring> XmlElement::get_attr_value(std::wstring attr) const noexcept
    {
        for (size_t i = 0u; i < m_attributes.size(); i++) {
            if (m_attributes[i].m_name == attr)
                return m_attributes[i].m_value;
        }
        return std::optional<std::wstring>();
    }

    const std::shared_ptr<XmlNode> XmlElement::get_child(size_t idx) const
    {
        return m_children.at(idx);
    }

    const std::vector<std::shared_ptr<XmlNode>>& XmlElement::get_children() const noexcept
    {
        return m_children;
    }

    void XmlElement::get_filtered_children(std::wstring tagname_filter,
        std::vector<std::shared_ptr<XmlElement>>& filtered_children) const noexcept
    {
        filtered_children.clear();

        for (std::shared_ptr<XmlNode> child : m_children)
        {
            try
            {
                std::shared_ptr<XmlElement> elem = dynamic_pointer_cast<XmlElement>(child);
                if (elem->get_name() == tagname_filter)
                    filtered_children.push_back(elem);
            }
            catch (const std::bad_cast&)
            {
                continue;
            }
        }
    }

    void XmlElement::set_name(std::wstring name) noexcept
    {
        m_tagname = name;
    }

    void XmlElement::add_attribute(XmlAttribute&& attr) noexcept
    {
        m_attributes.push_back(std::move(attr));
    }

    bool XmlElement::set_attr_value(std::wstring attr, std::wstring value) noexcept
    {
        for (size_t i = 0u; i < m_attributes.size(); i++) {
            if (m_attributes[i].m_name == attr) {
                m_attributes[i].m_value = value;
                return true;
            }
        }
        return false;
    }

    void XmlElement::add_child(std::shared_ptr<XmlNode> child) noexcept
    {
        m_children.push_back(child);
    }

    std::wstring XmlElement::to_wstring(size_t indent_level, size_t indent_size) const noexcept
    {
        std::wostringstream wos;
        wos << std::wstring(indent_level * indent_size, L' ') << L"<" << m_tagname;
        for (XmlAttribute attr : m_attributes)
            wos << L' ' << attr.to_wstring();
        if (m_children.size() == 0u)
            wos << L"/>";
        else {
            wos << L">" << std::endl;
            for (std::shared_ptr<XmlNode> child : m_children)
                wos << child->to_wstring(indent_level + 1u, indent_size) << std::endl;
            wos << std::wstring(indent_level * indent_size, L' ') << L"</" << m_tagname << L">";
        }
        return wos.str();
    }
}
