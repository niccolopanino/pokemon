#pragma once
#include "SourcePosition.h"
#include "XmlNode.h"
#include <optional>
#include <string>

namespace pkmn::xml
{
    class XmlDeclaration : public XmlNode
    {
    public:
        XmlDeclaration(SourcePosition pos, std::wstring version,
            bool standalone = true) noexcept;
        XmlDeclaration(SourcePosition pos, std::wstring version,
            std::wstring encoding, bool standalone = true) noexcept;
    public:
        const std::wstring& get_version() const noexcept;
        const std::wstring& get_encoding() const noexcept;
        std::optional<bool> is_standalone() const noexcept;
        std::wstring to_wstring(size_t indent_level = 0u, size_t indent_size = 4u) const noexcept;
    private:
        std::wstring m_version;
        std::wstring m_encoding;
        std::optional<bool> m_standalone;
    };
}
