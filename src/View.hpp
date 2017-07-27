#ifndef FILE_TIMTRIS_VIEW_HPP
#define FILE_TIMTRIS_VIEW_HPP

#include <glm/glm.hpp>
#include <lp3/core.hpp>


namespace lp3 { namespace timtris {

class View {
public:
    View(lp3::core::MediaManager & media);
	~View();

    void operator()(const glm::mat4 & previous);

private:
    class Impl;

    Impl * impl;
};

}   }

#endif
