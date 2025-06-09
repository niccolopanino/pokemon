#pragma once
#include "HResBaseException.h"
#include <string>

namespace pkmn::gfx
{
	class HResWinException : public HResBaseException
	{
	public:
		HResWinException(int line, const wchar_t* file, HRESULT hr) noexcept;
	public:
		virtual const wchar_t* get_type() const noexcept override;
		virtual std::wstring get_error_description() const noexcept override;
		virtual const char* what() const noexcept override;
	};
}
