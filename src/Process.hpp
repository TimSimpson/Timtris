#ifndef FILE_TIMTRIS_PROCESS_HPP
#define FILE_TIMTRIS_PROCESS_HPP

#include "Controller.hpp"

namespace lp3 { namespace timtris {

// Represents a unique screen or subroutine of the game, like the title
// screen, game over screen, etc.
class Process {
public:
    virtual ~Process() = default;

    virtual gsl::owner<Process *> update(
        std::int64_t ms, ControllerManager & controls) = 0;
};


class ProcessRunner
// --------------------------------------------------------------------
// Game
// --------------------------------------------------------------------
//      For now this is where the bulk of the code from NND3d2.bas
//      will live. The state from that mammoth module is represented
//      by the World struct.
// --------------------------------------------------------------------
class Game
{
public:
    Game(GameContext game_context, GameProcessEntry start_proc);

    ~Game();

    void handle_input(const input::Event & even);

    bool quit() const;

    void update();

private:
    GameProcessSpace process;
    bool _quit;
};

}   }

#endif
