#pragma once
#include "XmlParserException.h"
#include "../elements/SourcePosition.h"
#include <string>

namespace pkmn::xml
{
	class UnexpectedCharSequenceException : public XmlParserException
	{
	public:
		UnexpectedCharSequenceException(int line, const wchar_t* file, const SourcePosition& pos,
			const std::wstring& expected, const std::wstring& actual) noexcept;
	public:
		virtual const wchar_t* get_type() const noexcept override;
		const std::wstring& get_expected_string() const noexcept;
		const std::wstring& get_actual_string() const noexcept;
		virtual const char* what() const noexcept override;
	private:
		std::wstring m_expected;
		std::wstring m_actual;
	};
}
