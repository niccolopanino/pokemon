#include "GfxInfoException.h"
#include "../../util/StringConv.h"
#include <sstream>

namespace pkmn::gfx
{
    GfxInfoException::GfxInfoException(int line, const wchar_t* file,
        const std::vector<std::wstring>& info_msgs) noexcept
        :
        BaseException(line, file)
    {
        // join all info messages with newlines into single string
        for (const std::wstring& msg : info_msgs) {
            m_info += msg;
            m_info.push_back(L'\n');
        }
        // remove final newline if exists
        if (!m_info.empty())
            m_info.pop_back();
    }

    const wchar_t* GfxInfoException::get_type() const noexcept
    {
        return L"Pokémon Graphics Info Exception";
    }

    std::wstring GfxInfoException::get_error_info() const noexcept
    {
        return m_info;
    }

    const char* GfxInfoException::what() const noexcept
    {
        std::wostringstream wos;
        wos << get_type() << std::endl << std::endl
            << L"[Error Info]\n" << get_error_info() << std::endl << std::endl
            << get_origin_string();
        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
