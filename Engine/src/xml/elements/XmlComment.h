#pragma once
#include "SourcePosition.h"
#include "XmlMisc.h"
#include "XmlNode.h"
#include <memory>
#include <string>

namespace pkmn::xml::elem
{
	class XmlComment : public XmlMisc
	{
	public:
		XmlComment(SourcePosition pos, const std::wstring& comment,
			std::shared_ptr<XmlNode> parent = nullptr) noexcept;
	public:
		const std::wstring& get_comment() const noexcept;
		virtual std::wstring to_wstring(size_t indent_level = 0u,
			size_t indent_size = 4u) const noexcept;
	private:
		std::wstring m_comment;
	};
}
