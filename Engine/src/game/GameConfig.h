#pragma once
#include <string>

namespace pkmn
{
	class GameConfig
	{
	public:
		static GameConfig from_xml_file(const std::wstring& path);
	public:
		const std::wstring& get_title() const noexcept;
	private:
		std::wstring m_title;
	};
}
