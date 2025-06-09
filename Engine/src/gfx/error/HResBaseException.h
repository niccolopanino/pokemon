#pragma once
#include "../../error/BaseException.h"
#include "../../util/Windoof.h"
#include <string>

namespace pkmn::gfx
{
    class HResBaseException : public BaseException
    {
    public:
        static std::wstring translate_error_code(HRESULT hr) noexcept;
    public:
        HRESULT get_error_code() const noexcept;
    protected:
        HResBaseException(int line, const wchar_t* file, HRESULT hr) noexcept;
    protected:
        virtual const wchar_t* get_type() const noexcept override;
        virtual std::wstring get_error_description() const noexcept;
        virtual const char* what() const noexcept override;
    protected:
        HRESULT m_hr;
    };
}
