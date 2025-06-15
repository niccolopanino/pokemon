#pragma once
#include "../elements/SourcePosition.h"
#include "../../error/BaseException.h"
#include <optional>
#include <string>

namespace pkmn::xml
{
	class XmlParserException : public BaseException
	{
	public:
		XmlParserException(int line, const wchar_t* file, const SourcePosition& pos,
			const std::optional<std::wstring>& note = { }) noexcept;
	public:
		virtual const wchar_t* get_type() const noexcept override;
		const SourcePosition& get_src_pos() const noexcept;
		std::wstring get_src_pos_string() const noexcept;
		const std::optional<std::wstring>& get_note() const noexcept;
		virtual const char* what() const noexcept override;
	private:
		SourcePosition m_pos;
		std::optional<std::wstring> m_note;
	};
}
