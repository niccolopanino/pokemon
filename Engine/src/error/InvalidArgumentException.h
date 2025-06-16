#pragma once
#include "BaseException.h"
#include <optional>
#include <string>

namespace pkmn
{
	class InvalidArgumentException : public BaseException
	{
	public:
		InvalidArgumentException(int line, const wchar_t* file, std::wstring note,
			std::optional<std::wstring> argname = { }) noexcept;
	public:
		virtual const wchar_t* get_type() const noexcept override;
		const std::wstring& get_note() const noexcept;
		virtual const char* what() const noexcept override;
	private:
		std::wstring m_note;
		std::optional<std::wstring> m_argname;
	};
}
