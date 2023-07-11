#pragma once
#include "all_in_one.hpp"
using namespace sf;

class Setting {
public:
	int key_left, key_right, key_up, key_down, key_set, key_close;
	int key_cw, key_ccw, key_half;
	int key_hold;
	int ARR, DAS, SDF;
	Setting() : key_left(Keyboard::J), key_right(Keyboard::L), key_up(Keyboard::I), key_down(Keyboard::K), key_set(Keyboard::Space),
				key_close(Keyboard::X), ARR(0), DAS(7), SDF(6), key_cw(Keyboard::F), key_ccw(Keyboard::D), key_half(Keyboard::S),
				key_hold(Keyboard::E) {
	}
};

