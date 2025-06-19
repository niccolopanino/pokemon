#include "error/BaseException.h"
#include "game/Game.h"
#include "util/StringConv.h"
#include <exception>
#include <sal.h>
#include <Windows.h>

int WINAPI WinMain(_In_ HINSTANCE hinst, _In_opt_ HINSTANCE hprev_instance, _In_ LPSTR lp_cmd_line, _In_ int ncmd_show)
{
    try {
        return pkmn::game::Game().run();
    }
    catch (const pkmn::BaseException& ex) {
        MessageBoxW(nullptr, pkmn::str_to_wstr(ex.what()).c_str(), ex.get_type(),
            MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& ex) {
        MessageBoxW(nullptr, pkmn::str_to_wstr(ex.what()).c_str(), L"Standard Exception",
            MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBoxW(nullptr, L"No details available", L"Unknown Exception",
            MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}
