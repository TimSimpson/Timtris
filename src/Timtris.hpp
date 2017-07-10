#ifndef FILE_TIMTRIS_VIEW_HPP
#define FILE_TIMTRIS_VIEW_HPP

#include <glm/glm.hpp>
#include "Controller.hpp"


namespace lp3 { namespace timtris {

class Game {
public:
	Game(lp3::core::MediaManager & media);
	~Game();

    void operator()(const glm::mat4 & previous);

	void update(std::int64_t ms, ControllerManager & controls);
private:
    class Impl;

    Impl * impl;
};

}   }

#endif
