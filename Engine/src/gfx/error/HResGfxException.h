#pragma once
#include "HResBaseException.h"
#include <string>
#include <vector>

namespace pkmn::gfx
{
    class HResGfxException : public HResBaseException
    {
    public:
        HResGfxException(int line, const wchar_t* file, HRESULT hr,
            const std::vector<std::wstring>& info_msgs) noexcept;
    public:
        virtual const wchar_t* get_type() const noexcept override;
        std::wstring get_error_string() const noexcept;
        virtual std::wstring get_error_description() const noexcept override;
        std::wstring get_error_info() const noexcept;
        virtual const char* what() const noexcept override;
    private:
        std::wstring m_info;
    };
}
