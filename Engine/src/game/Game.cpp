#include "Game.h"
#include "GameConfig.h"
#include "../gfx/Window.h"
#include "../io/Keyboard.h"
#include <optional>
#include <string>

namespace pkmn::game
{
    Game::Game() : m_wnd(1024, 768, L"This should not appear")
    { }

    Game::~Game()
    { }

    int Game::run()
    {
        GameConfig config = GameConfig::from_xml_file(L"resources/gameconfig.xml");
        m_wnd.set_title(config.get_title().c_str());

        while (true) {
            // process all messages pending, but don't block for new messages
            if (const std::optional<int> ecode = Window::process_messages()) {
                // if return optional has value means we're quitting so return exit code
                return *ecode;
            }
            do_frame();
        }
    }

    void Game::do_frame()
    {
        while (const std::optional<Keyboard::Event> e = m_wnd.m_kbd.read_key())
        {
            if (!e->is_press())
                continue;

            switch (e->get_code())
            {
            case VK_ESCAPE:
                PostQuitMessage(0);
                break;
            }
        }

        m_wnd.gfx().begin_frame(.07f, .0f, .12f);
        m_wnd.gfx().end_frame();
    }
}
