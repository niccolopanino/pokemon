#include "SourcePosition.h"
#include "XmlMisc.h"
#include "XmlNode.h"
#include "XmlProcessingInstruction.h"
#include "../../util/Macros.h"
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

namespace pkmn::xml
{
    XmlProcessingInstruction::XmlProcessingInstruction(SourcePosition pos, std::wstring target,
        std::shared_ptr<XmlNode> parent, std::optional<std::wstring> data) noexcept
        : XmlMisc(pos, parent), m_targetname(target), m_data(data)
    { }

    const std::wstring& XmlProcessingInstruction::get_target() const noexcept
    {
        return m_targetname;
    }

    const std::optional<std::wstring>& XmlProcessingInstruction::get_data() const noexcept
    {
        return m_data;
    }

    std::wstring XmlProcessingInstruction::to_wstring(size_t indent_level,
        size_t indent_size) const noexcept
    {
        std::wostringstream wos;
        wos << L"<?" << m_targetname;
        if (m_data)
            wos << L" " << *m_data;
        wos << L"?>";
        return wos.str();
    }

    void XmlProcessingInstruction::check_targetname() const
    {
        std::wstring up = m_targetname;
        std::transform(up.begin(), up.end(), up.begin(), std::towlower);
        if (up == L"xml") {
            throw INVALIDARG(L"The target of a xml processing instruction must not be \"xml\" "
                "/ \"XML\" / ...! Use XmlDeclaration instead.");
        }
    }
}
