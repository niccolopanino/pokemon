#pragma once
#include "XmlDeclaration.h"
#include "XmlElement.h"
#include "XmlMisc.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace pkmn::xml
{
	class XmlDocument
	{
	public:
		XmlDocument() noexcept = default;
		XmlDocument(std::shared_ptr<XmlElement> root, std::optional<XmlDeclaration> decl = { }) noexcept;
	public:
		const std::optional<XmlDeclaration>& get_xml_declaration() const noexcept;
		void set_xml_decl(XmlDeclaration&& decl) noexcept;
		std::shared_ptr<XmlElement> get_root() const noexcept;
		void set_root(std::shared_ptr<XmlElement> root) noexcept;
		const std::vector<std::shared_ptr<XmlMisc>>& get_pre_root_misc_stuff() const noexcept;
		void add_misc_stuff_pre_root(std::shared_ptr<XmlMisc> misc) noexcept;
		const std::vector<std::shared_ptr<XmlMisc>>& get_post_root_misc_stuff() const noexcept;
		void add_misc_stuff_post_root(std::shared_ptr<XmlMisc> misc) noexcept;
		std::wstring to_wstring() const noexcept;
	private:
		std::optional<XmlDeclaration> m_decl;
		std::shared_ptr<XmlElement> m_root;
		std::vector<std::shared_ptr<XmlMisc>> m_pre_root_misc;
		std::vector<std::shared_ptr<XmlMisc>> m_post_root_misc;
	};
}
