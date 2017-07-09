#ifndef FILE_TIMTRIS_CONTROLLER_HPP
#define FILE_TIMTRIS_CONTROLLER_HPP

#include <lp3/input.hpp>

namespace lp3 { namespace timtris {

// Maps controls very specifically to this game
class Pad {
public:
	Pad(lp3::input::Control & c)
    :   control(c)
    {}

	inline bool accept() const {
		return control.state(4) || control.state(6);
	}

	inline bool cancel() const {
		return control.state(5);
	}

    inline bool down() const {
        return control.state(1);
    }

	inline bool flip() const {
		return control.state(4) || control.state(5); 
	}

    inline bool left() const {
        return control.state(2);
    }

    inline bool right() const {
        return control.state(3);
    }
	
	inline bool start() const {
		return control.state(6);
	}

    inline bool up() const {
        return control.state(0);
    }
	
private:
    lp3::input::Control & control;
};


class ControllerManager {
public:
    ControllerManager();

	// True if any controllers are hitting accept.
	bool any_accept();

    void update();
private:
	lp3::input::Controls controls;
	std::vector<std::unique_ptr<Pad>> pads;
};

}   }

#endif
