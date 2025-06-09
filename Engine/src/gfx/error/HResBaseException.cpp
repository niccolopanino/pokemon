#include "HResBaseException.h"
#include "../../util/StringConv.h"
#include <sstream>

namespace pkmn::gfx
{
	std::wstring HResBaseException::translate_error_code(HRESULT hr) noexcept
	{
		wchar_t* msg_buf = nullptr;

        // WinAPI will allocate memory for error string
        // and make our pointer point to it
        const DWORD msg_len = FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPWSTR>(&msg_buf), 0, nullptr
        );

        // 0 string length returned indicates failure
        if (msg_len == 0)
            return L"Unidentified error code";

        // copy error string from WinAPI-allocated buffer to std::wstring
        std::wstring err_str = msg_buf;
        // free WinAPI buffer
        LocalFree(msg_buf);

        return err_str;
	}

    HRESULT HResBaseException::get_error_code() const noexcept
    {
        return m_hr;
    }

    HResBaseException::HResBaseException(int line, const wchar_t* file, HRESULT hr) noexcept :
        m_hr(hr), BaseException(line, file)
    {
    }

    const wchar_t* HResBaseException::get_type() const noexcept
    {
        return L"Pokémon Base HResult Exception";
    }

    std::wstring HResBaseException::get_error_description() const noexcept
    {
        return translate_error_code(m_hr);
    }

    const char* HResBaseException::what() const noexcept
    {
        std::wostringstream wos;
        wos << get_type() << std::endl
            << L"[Error Code] 0x" << std::hex << std::uppercase << get_error_code() << std::dec
            << " (" << (unsigned long)get_error_code() << ")" << std::endl
            << "[Desccription] " << get_error_description() << std::endl
            << get_origin_string();
        m_what_buffer = wstr_to_str(wos.str());
        return m_what_buffer.c_str();
    }
}
