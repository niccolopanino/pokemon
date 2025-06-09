#pragma once
#include "../gfx/Window.h"

namespace pkmn::game
{
	class Game
	{
	public:
		Game();
		~Game();
	public:
		int run();
	private:
		void do_frame();
	private:
		Window m_wnd;
	};
}
