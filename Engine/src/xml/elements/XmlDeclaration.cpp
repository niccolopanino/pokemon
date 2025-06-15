#include "SourcePosition.h"
#include "XmlDeclaration.h"
#include <optional>
#include <sstream>
#include <string>

namespace pkmn::xml
{
    XmlDeclaration::XmlDeclaration(SourcePosition pos, const std::wstring& version,
        bool standalone) noexcept
        : m_pos(pos), m_version(version), m_standalone(standalone)
    { }

    XmlDeclaration::XmlDeclaration(SourcePosition pos, const std::wstring& version,
        const std::wstring& encoding, bool standalone) noexcept
        : m_pos(pos), m_version(version), m_encoding(encoding), m_standalone(standalone)
    { }

    const std::wstring& XmlDeclaration::get_version() const noexcept
    {
        return m_version;
    }

    const std::wstring& XmlDeclaration::get_encoding() const noexcept
    {
        return m_encoding;
    }

    std::optional<bool> XmlDeclaration::is_standalone() const noexcept
    {
        return m_standalone ? std::optional<bool>(*m_standalone) : std::optional<bool>();
    }

    std::wstring XmlDeclaration::to_wstring(size_t indent_level, size_t indent_size) const noexcept
    {
        std::wostringstream wos;
        wos << L"<?xml version=\"" << m_version << L"\"";
        if (m_encoding.length() > 0u)
            wos << L" encoding=\"" << m_encoding << L"\"";
        if (m_standalone)
            wos << L" standalone=\"" << (*m_standalone ? L"yes" : L"no") << L"\"";
        wos << L"?>";
        return wos.str();
    }
}
