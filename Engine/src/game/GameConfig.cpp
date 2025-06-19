#include "GameConfig.h"
#include "../util/Macros.h"
#include "../xml/elements/XmlDocument.h"
#include "../xml/elements/XmlElement.h"
#include "../xml/elements/XmlText.h"
#include "../xml/parser/XmlParser.h"
#include <memory>
#include <string>
#include <vector>

namespace pkmn
{
    GameConfig GameConfig::from_xml_file(const std::wstring& path)
    {
        GameConfig config;
        xml::XmlDocument xmldoc = xml::XmlParser::parse_file(path);

        std::shared_ptr<xml::XmlElement> root = xmldoc.get_root();
        if (root->get_name() != L"config")
            throw INVALIDARG(L"Gameconfig xml file must contain a root element named \'config\'!");

        std::vector<std::shared_ptr<xml::XmlElement>> children;
        root->get_filtered_children(L"title", children);
        if (children.size() != 1)
            throw INVALIDARG(L"Gameconfig xml file must contain exactly one \'title\' element!");

        std::vector<std::shared_ptr<xml::XmlText>> text_children;
        children.at(0)->get_children_by_type(text_children);
        if (text_children.size() != 1)
            throw INVALIDARG(L"Gameconfig xml file contains malformed title element.");
        config.m_title = text_children.at(0)->get_text();

        return config;
    }

    const std::wstring& GameConfig::get_title() const noexcept
    {
        return m_title;
    }
}
