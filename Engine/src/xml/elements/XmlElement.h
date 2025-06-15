#pragma once
#include "SourcePosition.h"
#include "XmlNode.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace pkmn::xml
{
    struct XmlAttribute
    {
    public:
        XmlAttribute(const std::wstring& name, const std::wstring& value) noexcept;
        XmlAttribute(std::wstring&& name, std::wstring&& value) noexcept;
        XmlAttribute(const XmlAttribute& attr) noexcept;
        XmlAttribute(XmlAttribute&& attr) noexcept;
    public:
        XmlAttribute& operator=(const XmlAttribute& attr) noexcept;
        XmlAttribute& operator=(XmlAttribute&& attr) noexcept;
    public:
        std::wstring to_wstring() const noexcept;
    public:
        std::wstring m_name;
        std::wstring m_value;
    };

    class XmlElement : public XmlNode
    {
    public:
        XmlElement(SourcePosition pos, const std::wstring& tag,
            std::shared_ptr<XmlNode> parent = nullptr) noexcept;
        XmlElement(SourcePosition pos, const std::wstring& tag,
            const std::vector<XmlAttribute>& attr, std::shared_ptr<XmlNode> parent = nullptr) noexcept;
    public:
        const std::wstring& get_name() const noexcept;
        const std::vector<XmlAttribute>& get_attributes() const noexcept;
        std::optional<std::wstring> get_attr_value(const std::wstring& attr) const noexcept;
        const std::shared_ptr<XmlNode> get_child(size_t idx) const;
        const std::vector<std::shared_ptr<XmlNode>>& get_children() const noexcept;
        std::vector<std::shared_ptr<XmlElement>> get_filtered_children(const std::wstring& tagname_filter) const noexcept;
        void set_name(const std::wstring& name) noexcept;
        void add_attribute(const XmlAttribute& attr) noexcept;
        bool set_attr_value(const std::wstring& attr, const std::wstring& value) noexcept;
        void add_child(std::shared_ptr<XmlNode> child) noexcept;
        virtual std::wstring to_wstring(size_t indent_level = 0u, size_t indent_size = 4u) const noexcept override;
    private:
        std::wstring m_tagname;
        std::vector<XmlAttribute> m_attributes;
        std::vector<std::shared_ptr<XmlNode>> m_children;
    };
}
