#pragma once
#include "../../error/BaseException.h"
#include <string>
#include <vector>

namespace pkmn::gfx
{
	class GfxInfoException : public BaseException
	{
    public:
        GfxInfoException(int line, const wchar_t* file,
            const std::vector<std::wstring>& info_msgs) noexcept;
        virtual const wchar_t* get_type() const noexcept override;
        std::wstring get_error_info() const noexcept;
        virtual const char* what() const noexcept override;
    private:
        std::wstring m_info;
	};
}
