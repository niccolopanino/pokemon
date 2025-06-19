#pragma once
#include "SourcePosition.h"
#include "XmlAttribute.h"
#include "XmlNode.h"
#include <memory>
#include <optional>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <vector>

namespace pkmn::xml
{
    class XmlElement : public XmlNode
    {
    public:
        XmlElement(SourcePosition pos, std::wstring&& tag,
            std::shared_ptr<XmlNode> parent = nullptr) noexcept;
    public:
        const std::wstring& get_name() const noexcept;
        const std::vector<XmlAttribute>& get_attributes() const noexcept;
        std::optional<std::wstring> get_attr_value(std::wstring attr) const noexcept;
        const std::shared_ptr<XmlNode> get_child(size_t idx) const;
        const std::vector<std::shared_ptr<XmlNode>>& get_children() const noexcept;
        void get_filtered_children(std::wstring tagname_filter,
            std::vector<std::shared_ptr<XmlElement>>& filtered_children) const noexcept;
        void set_name(std::wstring name) noexcept;
        void add_attribute(XmlAttribute&& attr) noexcept;
        bool set_attr_value(std::wstring attr, std::wstring value) noexcept;
        void add_child(std::shared_ptr<XmlNode> child) noexcept;
        virtual std::wstring to_wstring(size_t indent_level = 0u, size_t indent_size = 4u) const noexcept override;
        template<typename T> void get_children_by_type(std::vector<std::shared_ptr<T>>& filtered_children) const noexcept;
    private:
        std::wstring m_tagname;
        std::vector<XmlAttribute> m_attributes;
        std::vector<std::shared_ptr<XmlNode>> m_children;
    };

    template<typename T>
    inline void XmlElement::get_children_by_type(std::vector<std::shared_ptr<T>>& filtered_children) const noexcept
    {
        static_assert(std::is_base_of<XmlNode, T>::value, L"Child types must derive from XmlNode");
        filtered_children.clear();
        for (std::shared_ptr<XmlNode> child : m_children) {
            try {
                std::shared_ptr<T> elem = dynamic_pointer_cast<T>(child);
                filtered_children.push_back(elem);
            }
            catch (const std::bad_cast&) {
                continue;
            }
        }
    }
}
