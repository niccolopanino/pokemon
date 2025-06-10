#include "XmlDeclaration.h"
#include "XmlDocument.h"
#include "XmlElement.h"
#include "XmlMisc.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace pkmn::xml::elem
{
    XmlDocument::XmlDocument(std::shared_ptr<XmlElement> root, std::optional<XmlDeclaration> decl) noexcept
        : m_root(root), m_decl(decl)
    { }

    const std::optional<XmlDeclaration>& XmlDocument::get_xml_declaration() const noexcept
    {
        return m_decl;
    }

    std::shared_ptr<XmlElement> XmlDocument::get_root() const noexcept
    {
        return m_root;
    }

    const std::vector<std::shared_ptr<XmlMisc>>& XmlDocument::get_pre_root_misc_stuff() const noexcept
    {
        return m_pre_root_misc;
    }

    const std::vector<std::shared_ptr<XmlMisc>>& XmlDocument::get_post_root_misc_stuff() const noexcept
    {
        return m_post_root_misc;
    }
}
