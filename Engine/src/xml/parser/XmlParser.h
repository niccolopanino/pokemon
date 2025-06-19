#pragma once
#include "ParserState.h"
#include "../elements/SourcePosition.h"
#include "../elements/XmlAttribute.h"
#include "../elements/XmlDeclaration.h"
#include "../elements/XmlDocument.h"
#include "../elements/XmlElement.h"
#include <memory>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

namespace pkmn::xml
{
	class XmlParser
	{
	public:
		static XmlDocument parse_file(const std::wstring& path);
	private:
		static void init();
	private:
		static bool s_initialized;
		static std::unordered_set<wchar_t> s_whitespaces;
	private:
		XmlParser(const std::wstring& path, std::wstring&& file_content) noexcept;
	private:
		void set_state(ParserState next_state) noexcept;
		void push_state(ParserState next_state);
		void pop_state() noexcept;
		wchar_t wpeek(bool dont_throw_weof = false);
		wchar_t wread(bool dont_throw_weof = false);
		void read_whitespace(bool dont_throw_weof = false);
		std::wstring read_identifier(const std::unordered_set<wchar_t>& special_delims);
		XmlAttribute read_attribute(const std::optional<std::wstring>& expected_attr_name = { });
		XmlDeclaration read_xml_decl(SourcePosition pos);
		std::optional<std::wstring> read_pi_data();
		std::wstring read_comment();
		XmlDocument read_xml_doc();
	private:
		XmlDocument m_doc;
		std::wistringstream m_wiss;
		SourcePosition m_src_pos;
		ParserState m_state;
		std::stack<ParserState> m_previous_states;
		std::stack<std::shared_ptr<XmlElement>> m_open_elements;
		std::vector<size_t> m_line_lengths;
	};
}
