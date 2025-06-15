#pragma once
#include "XmlParserException.h"
#include "../elements/SourcePosition.h"

namespace pkmn::xml
{
	class UnexpectedEofException : public XmlParserException
	{
	public:
		UnexpectedEofException(int line, const wchar_t* file, const SourcePosition& pos) noexcept;
	};
}
