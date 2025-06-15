#include "XmlDeclaration.h"
#include "XmlDocument.h"
#include "XmlElement.h"
#include "XmlMisc.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace pkmn::xml
{
    XmlDocument::XmlDocument(std::shared_ptr<XmlElement> root, std::optional<XmlDeclaration> decl) noexcept
        : m_root(root), m_decl(decl)
    { }

    const std::optional<XmlDeclaration>& XmlDocument::get_xml_declaration() const noexcept
    {
        return m_decl;
    }

    void XmlDocument::set_xml_decl(XmlDeclaration&& decl) noexcept
    {
        m_decl = decl;
    }

    std::shared_ptr<XmlElement> XmlDocument::get_root() const noexcept
    {
        return m_root;
    }

    void XmlDocument::set_root(std::shared_ptr<XmlElement> root) noexcept
    {
        m_root = root;
    }

    const std::vector<std::shared_ptr<XmlMisc>>& XmlDocument::get_pre_root_misc_stuff() const noexcept
    {
        return m_pre_root_misc;
    }

    void XmlDocument::add_misc_stuff_pre_root(std::shared_ptr<XmlMisc> misc) noexcept
    {
        m_pre_root_misc.push_back(misc);
    }

    const std::vector<std::shared_ptr<XmlMisc>>& XmlDocument::get_post_root_misc_stuff() const noexcept
    {
        return m_post_root_misc;
    }

    void XmlDocument::add_misc_stuff_post_root(std::shared_ptr<XmlMisc> misc) noexcept
    {
        m_post_root_misc.push_back(misc);
    }
}
