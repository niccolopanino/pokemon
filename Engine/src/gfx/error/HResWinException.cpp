#include "HResWinException.h"

namespace pkmn::gfx
{
    HResWinException::HResWinException(int line, const wchar_t* file, HRESULT hr) noexcept :
        HResBaseException(line, file, hr)
    {
    }

    const wchar_t* HResWinException::get_type() const noexcept
    {
        return L"Pokémon Window HResult Exception";
    }

    std::wstring HResWinException::get_error_description() const noexcept
    {
        return HResBaseException::get_error_description();
    }

    const char* HResWinException::what() const noexcept
    {
        return HResBaseException::what();
    }
}
