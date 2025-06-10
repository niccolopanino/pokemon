#include "Keyboard.h"
#include <bitset>
#include <optional>
#include <queue>

//------------------------------------------------------------------------------
//---- Keyboard Event stuff ----------------------------------------------------
//------------------------------------------------------------------------------

namespace pkmn
{
    Keyboard::Event::Event(Type type, unsigned char code) noexcept
        : m_type(type), m_code(code)
    { }

    bool Keyboard::Event::is_press() const noexcept
    {
        return m_type == Type::e_press;
    }

    bool Keyboard::Event::is_release() const noexcept
    {
        return m_type == Type::e_release;
    }

    unsigned char Keyboard::Event::get_code() const noexcept
    {
        return m_code;
    }
}

//------------------------------------------------------------------------------
//---- Keyboard stuff ----------------------------------------------------------
//------------------------------------------------------------------------------

namespace pkmn
{
    std::optional<Keyboard::Event> Keyboard::read_key() noexcept
    {
        if (m_keybuffer.size() > 0u) {
            Event e = m_keybuffer.front();
            m_keybuffer.pop();
            return e;
        }
        return std::optional<Event>();
    }

    std::optional<char> Keyboard::read_char() noexcept
    {
        if (m_charbuffer.size() > 0u) {
            unsigned char mander = m_charbuffer.front();
            m_charbuffer.pop();
            return mander;
        }
        return std::optional<char>();
    }

    bool Keyboard::is_key_empty() const noexcept
    {
        return m_keybuffer.empty();
    }
    
    bool Keyboard::is_char_empty() const noexcept
    {
        return m_charbuffer.empty();
    }

    bool Keyboard::is_key_pressed(unsigned char keycode) const noexcept
    {
        return m_keystates[keycode];
    }

    void Keyboard::flush_key() noexcept
    {
        m_keybuffer = std::queue<Event>();
    }

    void Keyboard::flush_char() noexcept
    {
        m_charbuffer = std::queue<char>();
    }

    void Keyboard::enable_autorepeat() noexcept
    {
        m_autorepeat_enabled = true;
    }

    void Keyboard::disable_autorepeat() noexcept
    {
        m_autorepeat_enabled = false;
    }

    bool Keyboard::is_autorepeat_enabled() const noexcept
    {
        return m_autorepeat_enabled;
    }

    void Keyboard::on_key_pressed(unsigned char keycode) noexcept
    {
        m_keystates[keycode] = true;
        m_keybuffer.push(Event(Event::Type::e_press, keycode));
        trim_buffer(m_keybuffer);
    }

    void Keyboard::on_key_released(unsigned char keycode) noexcept
    {
        m_keystates[keycode] = false;
        m_keybuffer.push(Event(Event::Type::e_release, keycode));
        trim_buffer(m_keybuffer);
    }

    void Keyboard::on_char(char mander) noexcept
    {
        m_charbuffer.push(mander);
        trim_buffer(m_charbuffer);
    }

    void Keyboard::clear_state() noexcept
    {
        m_keystates.reset();
    }
}
