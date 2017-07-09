#include "Controller.hpp"


namespace lp3 { namespace timtris {

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

}   }   // end namespace
