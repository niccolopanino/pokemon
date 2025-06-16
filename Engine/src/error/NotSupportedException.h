#pragma once
#include "BaseException.h"
#include <string>

namespace pkmn
{
	class NotSupportedException : public BaseException
	{
	public:
		NotSupportedException(int line, const wchar_t* file, std::wstring feature) noexcept;
	public:
		const std::wstring& get_feature() const noexcept;
		virtual const wchar_t* get_type() const noexcept override;
		virtual const char* what() const noexcept override;
	private:
		std::wstring m_feature;
	};
}
