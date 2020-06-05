#ifndef FILE_TIMTRIS_SCREENS_HPP
#define FILE_TIMTRIS_SCREENS_HPP

#include <lp3/core.hpp>
#include "../input/Controller.hpp"
#include "Screen.hpp"

namespace timtris { namespace screens {

std::unique_ptr<Screen> create_title_screen(lp3::core::MediaManager & media);
std::unique_ptr<Screen> create_options_screen(lp3::core::MediaManager & media,
	                                          ControllerManager & controls);

}   }

#endif
