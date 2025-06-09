#pragma once
#include <exception>
#include <string>

namespace pkmn
{
    class BaseException : public std::exception
    {
    public:
        BaseException(int line, const wchar_t* file) noexcept;
    public:
        virtual const wchar_t* get_type() const noexcept;
        int get_line() const noexcept;
        const std::wstring& get_file() const noexcept;
        std::wstring get_origin_string() const noexcept;
        virtual const char* what() const noexcept override;
    protected:
        mutable std::string m_what_buffer;
    private:
        int m_line;
        std::wstring m_file;
    };
}
