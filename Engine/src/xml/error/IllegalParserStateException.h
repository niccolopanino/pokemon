#pragma once
#include "XmlParserException.h"
#include "../elements/SourcePosition.h"
#include "../parser/ParserState.h"

namespace pkmn::xml
{
	class IllegalParserStateException : public XmlParserException
	{
	public:
		IllegalParserStateException(int line, const wchar_t* file, const SourcePosition& pos,
			ParserState state) noexcept;
	public:
		virtual const wchar_t* get_type() const noexcept override;
		ParserState get_state() const noexcept;
		virtual const char* what() const noexcept override;
	private:
		ParserState m_state;
	};
}
