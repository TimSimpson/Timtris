#include "Controller.hpp"


namespace timtris {

namespace input = lp3::input;

ControllerManager::ControllerManager()
:	controls{},
	pads{}
{
	input::PreferredButtonMapping game_pad_mapping;
	game_pad_mapping.device = input::PreferredDevice::GAME_PAD;
	game_pad_mapping.set_mapping(
		"D-pad Up", "D-pad Down", "D-pad Left", "D-pad Right",
		"A", "B", "Start");

	input::PreferredButtonMapping kb_mapping;
	kb_mapping.device = input::PreferredDevice::KEYBOARD;
	kb_mapping.set_mapping(
		"Up", "Down", "Left", "Right",
		"Z", "X", "Return");

	std::vector<input::PreferredButtonMapping> preferred_mappings = {
		game_pad_mapping, kb_mapping
	};

	controls.set_defaults(0, preferred_mappings);

	pads.push_back(std::make_unique<Pad>(controls.get_control(0)));
}

bool ControllerManager::any_accept() {
	for (std::unique_ptr<Pad> & p : pads) {
		if (p && p->accept()) {
			return true;
		}
	}
	return false;
}

void ControllerManager::update() {
	controls.update();
}


CursorPad::CursorPad()
{
}

void CursorPad::init(ControllerManager & controls) {
	back_off_time[0] = controls[0].up() ? 500 : 0;
	back_off_time[1] = controls[0].down() ? 500 : 0;
	back_off_time[2] = controls[0].left() ? 500 : 0;
	back_off_time[3] = controls[0].right() ? 500 : 0;
	back_off_time[4] = controls.any_accept() ? 500 : 0;
	back_off_time[5] = controls[0].cancel() ? 500 : 0;
	std::fill(state.begin(), state.end(), false);
}

bool CursorPad::accept() const {
	return state[4];
}

bool CursorPad::cancel() const {
	return state[5];
}

bool CursorPad::down() const {
	return state[1];
}

bool CursorPad::left() const {
	return state[2];
}

bool CursorPad::right() const {
	return state[3];
}

bool CursorPad::up() const {
	return state[0];
}

void CursorPad::update(ControllerManager & controls, const std::int64_t ms) {
	// Each key has a repeat value which goes down here...
	for (int & t : back_off_time) {
		if (t > 0) {
			t -= lp3::narrow<int>(ms);
		}
	}
	// if the keys aren't down, the limit goes to zero right away.
	if (!controls[0].up()) {
		back_off_time[0] = 0;
	}
	if (!controls[0].down()) {
		back_off_time[1] = 0;
	}
	if (!controls[0].left()) {
		back_off_time[2] = 0;
	}
	if (!controls[0].right()) {
		back_off_time[3] = 0;
	}
	if (!controls.any_accept()) {
		back_off_time[4] = 0;
	}
	if (!controls[0].cancel()) {
		back_off_time[5] = 0;
	}

	// The extra parenthesis below are to avoid very nice Clang warnings
	// on assignments in an if statement like this, and not some kind of
	// alt-right racist troll thing.
	if ((state[0] = (controls[0].up() && back_off_time[0] <= 0))) {
		back_off_time[0] = 500;
	}
	if ((state[1] = (controls[0].down() && back_off_time[1] <= 0))) {
		back_off_time[1] = 500;
	}
	if ((state[2] = (controls[0].left() && back_off_time[2] <= 0))) {
		back_off_time[2] = 500;
	}
	if ((state[3] = (controls[0].right() && back_off_time[3] <= 0))) {
		back_off_time[3] = 500;
	}
	if ((state[4] = (controls.any_accept() && back_off_time[4] <= 0))) {
		back_off_time[4] = 500;
	}
	if ((state[5] = (controls[0].cancel() && back_off_time[5] <= 0))) {
		back_off_time[5] = 500;
	}
}

}   // end namespace
