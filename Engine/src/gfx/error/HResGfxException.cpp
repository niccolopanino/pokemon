#include "HResGfxException.h"
#include "dxerr/dxerr.h"
#include "../../util/StringConv.h"
#include <sstream>

namespace pkmn::gfx
{
    HResGfxException::HResGfxException(int line, const wchar_t* file, HRESULT hr,
        const std::vector<std::wstring>& info_msgs) noexcept
        : HResBaseException(line, file, hr)
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

    const wchar_t* HResGfxException::get_type() const noexcept
    {
        return L"Pokémon Graphics HResult Exception";
    }

    std::wstring HResGfxException::get_error_string() const noexcept
    {
        return std::wstring(DXGetErrorStringW(m_hr));
    }

    std::wstring HResGfxException::get_error_description() const noexcept
    {
        wchar_t mander[512];
        DXGetErrorDescriptionW(m_hr, mander, sizeof(mander));
        return std::wstring(mander);
    }

    std::wstring HResGfxException::get_error_info() const noexcept
    {
        return m_info;
    }

    const char* HResGfxException::what() const noexcept
    {
        std::wostringstream wos;
        wos << get_type() << std::endl
            << L"[Error Code] 0x" << std::hex << std::uppercase << get_error_code() << std::dec
            << L" (" << (unsigned long)get_error_code() << ")" << std::endl
            << L"[Error String] " << get_error_string() << std::endl
            << L"[Description] " << get_error_description() << std::endl;

        if (!m_info.empty()) {
            wos << std::endl << L"[Error Info] " << std::endl << get_error_info()
                << std::endl << std::endl;
        }

        wos << get_origin_string();
        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
