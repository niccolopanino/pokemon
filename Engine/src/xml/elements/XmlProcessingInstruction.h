#pragma once
#include "SourcePosition.h"
#include "XmlMisc.h"
#include "XmlNode.h"
#include <memory>
#include <optional>
#include <string>

namespace pkmn::xml::elem
{
	class XmlProcessingInstruction : public XmlMisc
	{
	public:
		XmlProcessingInstruction(SourcePosition pos, const std::wstring& target,
			std::shared_ptr<XmlNode> parent, std::optional<std::wstring> data = { }) noexcept;
	public:
		const std::wstring& get_target() const noexcept;
		const std::optional<std::wstring>& get_data() const noexcept;
		virtual std::wstring to_wstring(size_t indent_level = 0u,
			size_t indent_size = 4u) const noexcept override;
	private:
		void check_targetname() const;
	private:
		std::wstring m_targetname;
		std::optional<std::wstring> m_data;
	};
}
