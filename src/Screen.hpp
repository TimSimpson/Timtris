#ifndef FILE_TIMTRIS_SCREEN_HPP

#define FILE_TIMTRIS_SCREEN_HPP
#include <glm/glm.hpp>
#include "Controller.hpp"


namespace lp3 { namespace timtris {

class Screen {
public:
    virtual ~Screen() {}
    virtual ScreenId id() const = 0;
    virtual void render(gfx::programs::SimpleTextured & program) = 0;
    virtual void start_up(ControllerManager & controls) = 0;
    virtual gsl::owner<Screen *> update(
        std::int64_t ms,
        ControllerManager & controls) = 0;
};

}   }

#endif
