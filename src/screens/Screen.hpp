#ifndef FILE_TIMTRIS_SCREEN_HPP

#define FILE_TIMTRIS_SCREEN_HPP
#include <glm/glm.hpp>
#include <lp3/gfx.hpp>
#include "../input/Controller.hpp"


namespace timtris {

class Screen {
public:
    virtual ~Screen() {}
    virtual void render(const glm::mat4 & previous) = 0;
    virtual gsl::owner<Screen *> update(
        std::int64_t ms,
        ControllerManager & controls) = 0;
};

}

#endif
